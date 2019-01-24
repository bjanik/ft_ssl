#!/bin/sh
echo MISSING INPUT FILE
./ft_ssl des-ecb -i
echo
echo MISSING OUTPUT FILE
./ft_ssl des-ecb -o 
echo

echo MISSING IV IN CBC/PCBC/BC
./ft_ssl des -k 23456
./ft_ssl des-bc -k 23456 
./ft_ssl des-pcbc -k 23456
./ft_ssl des-ecb -k 4567 -i auteur
echo

echo IV IS NOT HEX
./ft_ssl des -v 4567JK
echo

echo MISSING KEY ARGUMENT
./ft_ssl des-ecb -k
echo
echo KEY IS NOT HEX
./ft_ssl des -k 34567GG
echo

echo MISSING SALT ARGUMENT
./ft_ssl des-ecb -s
echo

echo MISSING PASSWORD ARGUMENT
./ft_ssl des -p 
echo

echo INVALID COMMAND
./ft_ssl toto -k 45678
echo

echo INPUT FILE IS A DIRECTORY
./ft_ssl des-ecb -k 456 -i libft
echo

echo OUTPUT FILE IS A DIRECTORY
mkdir directory
./ft_ssl des-ecb -k 34567 -i auteur -o directory
rmdir directory
echo

echo PERMISSION DENIED
touch denied && chmod 000 denied
./ft_ssl des-ecb -i denied 
./ft_ssl des-ecb -o denied
echo