#!/bin/bash

./ft_ssl sha1 Makefile
cat Makefile | shasum
echo ================== NEXT =================
./ft_ssl sha1 '/bin/ls'
shasum '/bin/ls'
echo ================== NEXT toto=================
echo -n toto | ./ft_ssl sha1 
echo -n toto | shasum
echo ================== NEXT =================
cat '/dev/null' | ./ft_ssl sha1
cat '/dev/null' | shasum
echo ================== NEXT =================
./ft_ssl sha1 toto tutu Makefile
shasum -a 1 toto tutu Makefile
echo ================== NEXT =================
cat Makefile /dev/null ft_ssl | ./ft_ssl sha1
cat Makefile /dev/null ft_ssl| shasum
echo ================== NEXT =================
./ft_ssl sha1 -s toto -r -s toto -q Makfile Makefile
echo ================== NEXT =================
cat ft_ssl /bin/ls /bin/cat | ./ft_ssl md5 -p
sleep 3 
cat ft_ssl /bin/ls /bin/cat | md5 -p 
sleep 3
echo ================== NEXT =================
cat ft_ssl /bin/ls /bin/cat | ./ft_ssl md5 -p |wc
cat ft_ssl /bin/ls /bin/cat | md5 -p |wc 
