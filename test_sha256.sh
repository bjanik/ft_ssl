#!/bin/bash
./ft_ssl sha256 Makefile
shasum -a 256 Makefile
echo ================== NEXT =================
./ft_ssl sha256 '/bin/ls'
shasum -a 256 '/bin/ls'
echo ================== NEXT =================
echo -n toto | ./ft_ssl sha256
echo -n toto | shasum -a 256
echo ================== NEXT =================
cat '/dev/null' | ./ft_ssl sha256
cat '/dev/null' | shasum -a 256
echo ================== NEXT =================
./ft_ssl sha256 toto tutu Makefile
shasum -a 256 toto tutu Makefile
echo ================== NEXT =================
cat Makefile /dev/null ft_ssl | ./ft_ssl sha256
cat Makefile /dev/null ft_ssl| shasum -a 256
echo ================== NEXT =================
./ft_ssl 
echo ================== NEXT =================
./ft_ssl unknown_command