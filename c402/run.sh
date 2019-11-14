clear
clear
make
echo "test begin
-----
"
./c402-test >output
#diff output c402-test.out
cat output
echo "
-----
test end
"
rm output
make clean
