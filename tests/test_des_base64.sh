#!/bin/sh
set -e
echo ----------------------------------
./ft_ssl des -i auteur -k 778855 -v CCDD55 -a | ./ft_ssl des -k 778855 -v CCDD55 -a -d > 42
diff 42 auteur
echo ----------------------------------
./ft_ssl des-bc -i /usr/bin/openssl -k 778855 -v CCAADD55 -a | ./ft_ssl des-bc -k 778855 -v CCAADD55 -a -d > 42
diff 42 /usr/bin/openssl
echo ----------------------------------
./ft_ssl des3-cbc -i /bin/bash -k 778855 -v CCAADD55FF45 -a | ./ft_ssl des3-cbc -k 778855 -v CCAADD55FF45 -a -d > 42
diff 42 /bin/bash
echo ----------------------------------
./ft_ssl des3-bc -i /bin/cat -k 778855 -v CCAADD55FF45 -a | ./ft_ssl des3-bc -k 778855 -v CCAADD55FF45 -a -d > 42
diff 42 /bin/cat
echo ----------------------------------

./ft_ssl des-ecb -i auteur -k 778855 -a  |  tr -d '\n'> 42
openssl des-ecb -in auteur -K 778855 -a |  tr -d '\n'  > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/ls -k 778855 -a |  tr -d '\n' > 42
openssl des-ecb -in /bin/ls -K 778855 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/bash -k 778855 -a |  tr -d '\n'> 42
openssl des-ecb -in /bin/bash -K 778855 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/cat -k 778855 -a |  tr -d '\n'> 42
openssl des-ecb -in /bin/cat -K 778855 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
echo ----------------------------------

echo -------TEST WITHOUT PIPES---------
./ft_ssl des -i auteur -k 778855 -v CCDD55 -a -o b64_auteur
./ft_ssl des -i b64_auteur -k 778855 -v CCDD55 -a -d -o o_auteur
diff auteur o_auteur
echo ----------------------------------
./ft_ssl des -i /bin/bash -k 778855 -v CCDD55 -a -o b64_bash
cat b64_bash | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_bash
diff /bin/bash o_bash
echo ----------------------------------
./ft_ssl des -i /bin/ls -k AA778855 -v D456CCDD55 -a -o b64_ls
./ft_ssl des -i b64_ls -k AA778855 -v D456CCDD55 -a -d -o o_ls
diff /bin/ls o_ls
echo ----------------------------------
./ft_ssl des -i /usr/bin/openssl -k 778855 -v CCDD55 -a -o b64_openssl
cat b64_openssl | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_openssl
diff /usr/bin/openssl o_openssl
echo ----------------------------------
./ft_ssl des -i /bin/cat -k 778855 -v CCDD55 -a -o b64_cat
cat b64_cat | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_cat
diff /bin/cat o_cat
echo ----------------------------------
./ft_ssl des -i ft_ssl -k 778855 -v CCDD55 -a -o b64_ftssl
cat b64_ftssl | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_ftssl
diff ft_ssl o_ftssl
echo ----------------------------------
./ft_ssl des -i /usr/bin/grep -k 778855 -v CCDD55 -a -o b64_grep
cat b64_grep | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_grep
diff /usr/bin/grep o_grep
echo ----------------------------------
./ft_ssl des -i /bin/ln -k 778855 -v CCDD55 -a -o b64_ln
cat b64_ln | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_ln
diff /bin/ln o_ln
echo ----------------------------------
./ft_ssl des -i /bin/ps -k 778855 -v CCDD55 -a -o b64_ps
cat b64_ps | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_ps
diff /bin/ps o_ps
echo ----------------------------------
./ft_ssl des -i /bin/kill -k 778855 -v CCDD55 -a -o b64_kill
cat b64_kill | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_kill
diff /bin/kill o_kill
echo ----------------------------------
./ft_ssl des -i /bin/pwd -k 778855 -v CCDD55 -a -o b64_pwd
cat b64_pwd | ./ft_ssl des -k 778855 -v CCDD55 -a -d -o o_pwd
diff /bin/pwd o_pwd
echo ----------------------------------

cat /dev/random | head -c 1697 > rand
./ft_ssl des -i rand -k 770000DD8855 -v ECDC55F5AE88 -a -o b64_rand
./ft_ssl des -i b64_rand -k 770000DD8855 -v ECDC55F5AE88 -a -d -o o_rand
diff rand o_rand
echo ----------------------------------

i=0
while [ "$i" -lt 10 ]
do
	cat /dev/random | head -c 7889 > rand
	./ft_ssl des -i rand -k 770000DD8855 -v ECDC55F5AE88 -a | tr -d '\n'> b64_rand
	openssl des -in rand -K 770000DD8855 -iv ECDC55F5AE88 -a | tr -d '\n' > open_rand
	diff b64_rand open_rand
	i=`expr $i + 1`
done
echo =======================================================================
i=0
while [ "$i" -lt 100 ]
do
	cat /dev/random | head -c 8243 > rand
	./ft_ssl des -i rand -k 770000DD8855 -v ECDC55F5AE88 -a -o b64_rand
	./ft_ssl des -i b64_rand -k 770000DD8855 -v ECDC55F5AE88 -a -d -o o_rand
	diff rand o_rand

	cat /dev/random | head -c 4579 > rand
	./ft_ssl des -i rand -k 770000DD8855 -v ECDC55F5AE88 -a -o b64_rand
	./ft_ssl des -i b64_rand -k 770000DD8855 -v ECDC55F5AE88 -a -d -o o_rand
	diff rand o_rand

	cat /dev/random | head -c 78050 > rand
	./ft_ssl des -i rand -k 770000DD8855 -v ECDC55F5AE88 -a -o b64_rand
	./ft_ssl des -i b64_rand -k 770000DD8855 -v ECDC55F5AE88 -a -d -o o_rand
	diff rand o_rand
	i=`expr $i + 1`
done
echo "ALL TESTS PASSED"
rm b64* o_* rand 42 43 open_rand
