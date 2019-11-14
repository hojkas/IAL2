clear
clear
make
echo "test begin
-----
"
./c401-test >output
#diff output c401-test.out
cat output
echo "
-----
test end
"
rm output
make clean
