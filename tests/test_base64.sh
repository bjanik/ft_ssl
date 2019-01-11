#!/bin/sh
set -e
i=0
while [ "$i" -lt 10 ]
do
	cat /dev/random | head -c 4568 > random
	./ft_ssl base64 -i random > 42
	base64 random > 43
	diff 42 43

	cat /dev/random | head -c 8975 > random
	./ft_ssl base64 -i random > 42
	base64 random > 43
	diff 42 43
	i=`expr $i + 1`

	cat /dev/random | head -c 9999 > random
	./ft_ssl base64 -i random > 42
	base64 random > 43
	diff 42 43
done

i=0
while [ "$i" -lt 100 ]
do
	cat /dev/random | head -c 4468 > random
	./ft_ssl base64 -i random | ./ft_ssl base64 -d > 42
	diff 42 random
	i=`expr $i + 1`
done
rm 42 43 random
echo "ALL TESTS PASSED"