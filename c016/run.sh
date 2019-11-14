clear
clear
make
echo "test begin\n"
./c016-test >output
diff output c016-test.out
rm output
echo "test end\n"
make clean
