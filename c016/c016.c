/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	for(int i = 0; i < HTSIZE; i++) {
		(*ptrht)[i] = NULL;
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	int index = hashCode(key);
	tHTItem *curr_item = (*ptrht)[index];
	while(curr_item != NULL) {
		//goes item after item in "line" of table, seeks the same key
		if(curr_item->key == key) return curr_item;
		curr_item = curr_item->ptrnext;
	}
	return NULL; //key wasn't found
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	tHTItem *to_place = htSearch(ptrht, key);
	if(to_place == NULL) {
		//if key is not found, hence new place needed
		//create new_item
		tHTItem* new_item = malloc(sizeof(struct tHTItem));
		new_item->key = key;
		new_item->data = data;
		new_item->ptrnext = NULL;

		int index = hashCode(key);
		to_place = (*ptrht)[index];
		//searches the "line" until the end is found, to_place points at empty end
		if(to_place == NULL) {
			//"line" is empty
			(*ptrht)[index] = new_item;
		}
		else {
			//"line" isn't empty, linking new item
			while(to_place->ptrnext != NULL) to_place = to_place->ptrnext;
			to_place->ptrnext = new_item;
		}
	}
	else {
		//key was found, just actualise data
		to_place->data = data;
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	tHTItem *to_read = htSearch(ptrht, key);
	if(to_read == NULL) return NULL;
	return &(to_read->data);
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	int index = hashCode(key);
	tHTItem *curr_item = (*ptrht)[index];
	//no item in line, end
	if(curr_item == NULL) return;

	if(curr_item->key == key) {
		//first item to be deleted
		tHTItem *next_item = curr_item->ptrnext;
		free(curr_item);
		(*ptrht)[index] = next_item;
	}
	else {
		//not first item to be deleted
		while(curr_item->ptrnext != NULL) {
			//goes item after item in "line" of table, seeks the same key
			if(curr_item->ptrnext->key == key) {
				//found that next item is the one to be deleted
				//sets next_item to item after the one to be deleted
				tHTItem *next_item = curr_item->ptrnext->ptrnext;
				free(curr_item->ptrnext);
				curr_item->ptrnext = next_item;
				return;
			}
			curr_item = curr_item->ptrnext;
		}
	}
	return; //no item with key found, did nothing
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	tHTItem *curr_item;
	tHTItem *next_item;
	for(int i = 0; i < HTSIZE; i++) {
		//for each line in table
		curr_item = (*ptrht)[i];
		while(curr_item != NULL) {
			next_item = curr_item->ptrnext;
			free(curr_item);
			curr_item = next_item;
		}
		(*ptrht)[i] = NULL;
	}
}
