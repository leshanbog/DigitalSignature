mkdir temp
cp build/ds temp && cd temp
cp ~/Documents/The_C__Programming_Language__Stroustrup_.pdf .

# test 1
printf "\n\n"
echo "pBn_2;1|v" | ./ds generate

# wrong password
echo "pBn_2;1|"  | ./ds sign The_C__Programming_Language__Stroustrup_.pdf --pr privatekey

./ds verify The_C__Programming_Language__Stroustrup_.pdf The_C__Programming_Language__Stroustrup__signature publickey

printf "\nTEST: Signarure must be NOT VALID\n\n\n"


# test2

# right password
echo "pBn_2;1|v" | ./ds sign The_C__Programming_Language__Stroustrup_.pdf --pr privatekey


./ds verify The_C__Programming_Language__Stroustrup_.pdf The_C__Programming_Language__Stroustrup__signature publickey

printf "\nTEST: Signarure must be VALID\n\n\n"


# test3

rm *key The_C__Programming_Language__Stroustrup__signature

echo "u7!WWb0O" | ./ds sign The_C__Programming_Language__Stroustrup_.pdf 

./ds verify The_C__Programming_Language__Stroustrup_.pdf The_C__Programming_Language__Stroustrup__signature publickey
printf "\nTEST: Signarure must be VALID\n\n\n"


# test4

rm The_C__Programming_Language__Stroustrup__signature

# wrong password
echo "u7!Wb0O" | ./ds sign The_C__Programming_Language__Stroustrup_.pdf --pr privatekey

./ds verify The_C__Programming_Language__Stroustrup_.pdf The_C__Programming_Language__Stroustrup__signature publickey
printf "\nTEST: Signarure must be NOT VALID\n\n\n"


# test5 

rm The_C__Programming_Language__Stroustrup__signature

echo "u7!WWb0O" | ./ds sign The_C__Programming_Language__Stroustrup_.pdf --pr privatekey

./ds verify The_C__Programming_Language__Stroustrup_.pdf The_C__Programming_Language__Stroustrup__signature publickey
printf "\nTEST: Signarure must be VALID\n\n\n"

cd ..
rm -r temp
