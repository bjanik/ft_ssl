#!/bin/sh
set -e
# echo ----------------------------------
# ./ft_ssl des-ecb -i auteur -p toto -s 666 > 42
# openssl des-ecb -in auteur -pass "pass:toto" -S 666 > 43
# diff 42 43
# echo ----------------------------------
# ./ft_ssl des-ecb -i auteur -p ' ' -s 6668887ADCB00000000 > 42
# openssl des-ecb -in auteur -pass "pass: " -S 6668887ADCB00000 > 43
# diff 42 43
# echo ----------------------------------
# ./ft_ssl des-ecb -i auteur -s 6668887ADCB00000 > 42
# openssl des-ecb -in auteur -S 6668887ADCB00000 > 43
# diff 42 43
# echo ----------------------------------
# ./ft_ssl des-ecb -i /bin/bash -s 6668887ADCB00000 -a |tr -d '\n' > 42
# openssl des-ecb -in /bin/bash -S 6668887ADCB00000 -a  | tr -d '\n' > 43
# diff 42 43
# echo ----------------------------------
# ./ft_ssl des3-ecb -i Makefile | ./ft_ssl des3-ecb -d > 42
# diff 42 Makefile
# echo ----------------------------------
# ./ft_ssl des-ecb -i Makefile -s 66667777 -o toto
# ./ft_ssl des-ecb -i toto -d > 42
# diff 42 Makefile
# echo ----------------------------------
# ./ft_ssl des3 -i ft_ssl -o toto
# ./ft_ssl des3 -d -i toto > 42
# diff 42 ft_ssl
# echo ----------------------------------
# ./ft_ssl des3 -i /bin/ls -o toto
# ./ft_ssl des3 -d -i toto  > 42
# diff 42 /bin/ls
# echo ----------------------------------
# ./ft_ssl des3 -i /bin/bash -o toto
# ./ft_ssl des3 -d -i toto > 42
# diff 42 /bin/bash

nb=1000
for i in {0..100}
do
	./ft_ssl des-ecb -i auteur -p 67776667678GHGJGY  -o toto
	./ft_ssl des-ecb -d -i toto -p 67776667678GHGJGY > 42
	diff 42 auteur
	./ft_ssl des3-ecb -i ft_ssl -p 67776667678GHGJGY -o toto
	./ft_ssl des3-ecb -d -i toto -p 67776667678GHGJGY > 42
	diff 42 ft_ssl
	./ft_ssl des -i Makefile -p 67776667678GHGJGY -o toto
	./ft_ssl des -d -i toto -p 67776667678GHGJGY > 42
	diff 42 Makefile
	./ft_ssl des3 -i Makefile -p PPP6777666 -o toto
	./ft_ssl des3 -d -i toto -p PPP6777666 > 42
	diff 42 Makefile
	./ft_ssl des3 -i auteur -p PPP6777666 -a -o toto
	./ft_ssl des3 -d -i toto -a -p PPP6777666 > 42
	diff 42 auteur


	echo $i
done
echo 'ALL TESTS PASSED!'
