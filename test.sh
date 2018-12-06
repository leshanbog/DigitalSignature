mkdir temp
cp build/ds temp && cd temp
doc=$1
cp -v ${doc} .
filename2="${doc##*/}"
filename="${filename2%.*}"
printf $filename

# test 1
printf "\n\n"
echo "pBn_2;1|v pBn_2;1|v" | ./ds generate

# wrong password
echo "pBn_2;1| pBn_2;1|"  | ./ds sign ${filename2} --pr privatekey

./ds verify ${filename2} ${filename}_signature publickey

printf "\nTEST: Signarure must be NOT VALID\n\n\n\n"

sleep 1
# test2

# right password
echo "pBn_2;1|v pBn_2;1|v" | ./ds sign ${filename2} --pr privatekey


./ds verify ${filename2} ${filename}_signature publickey

printf "\nTEST: Signarure must be VALID\n\n\n\n"
sleep 1

# test3

rm *key ${filename}_signature

echo "u7!WWb0O u7!WWb0O" | ./ds sign ${filename2} 

./ds verify ${filename2} ${filename}_signature publickey
printf "\nTEST: Signarure must be VALID\n\n\n\n"
sleep 1

# test4

rm ${filename}_signature

# wrong password
echo "u7!Wb0O u7!Wb0O" | ./ds sign ${filename2} --pr privatekey

./ds verify ${filename2} ${filename}_signature publickey
printf "\nTEST: Signarure must be NOT VALID\n\n\n\n"
sleep 1

# test5 

rm ${filename}_signature

echo "u7!WWb0O u7!WWb0O" | ./ds sign ${filename2} --pr privatekey

./ds verify ${filename2} ${filename}_signature publickey
printf "\nTEST: Signarure must be VALID\n\n\n\n"
sleep 1

# test6

echo "11" > my_test

echo "pas.5 pas.5" | ./ds sign my_test
./ds verify my_test my_test_signature publickey
printf "\nTEST: Signature must be VALID\n\n\n'n"
sleep 1


rm my_test
./ds verify my_test my_test_signature publickey
printf "\nTEST: Signature must be NOT VALID\n\n\n\n"
sleep 1


echo "10" > my_test
./ds verify my_test my_test_signature publickey
printf "\nTEST: Signature must be NOT VALID\n\n\n\n"
sleep 1

cd ..
rm -r temp