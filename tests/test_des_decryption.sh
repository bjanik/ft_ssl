#!/bin/bash
echo DECRYPTION DES-CBC
echo -----------------------------------------------
./ft_ssl des -i auteur -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des -k FEDCBA888 -v BBFFEE66 -d > ft
diff ft auteur
echo -----------------------------------------------
./ft_ssl des -i /bin/ls -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des -k FEDCBA888 -v BBFFEE66 -d > ft
diff /bin/ls ft
echo -----------------------------------------------
./ft_ssl des -i /dev/null -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des -k FEDCBA888 -v BBFFEE66 -d > ft
diff /dev/null ft
echo -----------------------------------------------
cat /dev/random | head -c 465879 > random
cat random | ./ft_ssl des -i random -k FED777AACBA888 -v CCFFEE66 | ./ft_ssl des -k FED777AACBA888 -v CCFFEE66 -d > ft
diff random ft
echo -----------------------------------------------
echo DECRYPTION DES3-CBC
./ft_ssl des3 -i auteur -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3 -k FEDCBA888 -v BBFFEE66 -d > ft
diff ft auteur
echo -----------------------------------------------
./ft_ssl des3 -i /bin/ls -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3 -k FEDCBA888 -v BBFFEE66 -d > ft
diff /bin/ls ft
echo -----------------------------------------------
./ft_ssl des3 -i /dev/null -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3 -k FEDCBA888 -v BBFFEE66 -d > ft
diff /dev/null ft
echo -----------------------------------------------
cat random | ./ft_ssl des -i random -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des -k FEDCBA888 -v BBFFEE66 -d > ft
diff random ft
echo DECRYPTION DES-ECB
echo -----------------------------------------------
./ft_ssl des-ecb -i auteur -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff ft auteur
echo -----------------------------------------------
./ft_ssl des-ecb -i /bin/ls -k FEBA888 -v BBFFEE66 | ./ft_ssl des-ecb -k FEBA888 -v BBFFEE66 -d > ft
diff /bin/ls ft
echo -----------------------------------------------
./ft_ssl des-ecb -i /dev/null -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff /dev/null ft
echo -----------------------------------------------
cat /dev/random | head -c 465879 > random
cat random | ./ft_ssl des-ecb -i random -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff random ft
echo DECRYPTION DES3-ECB
echo -----------------------------------------------
./ft_ssl des3-ecb -i auteur -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff ft auteur
echo -----------------------------------------------
./ft_ssl des3-ecb -i /bin/ls -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff /bin/ls ft
echo -----------------------------------------------
./ft_ssl des3-ecb -i /dev/null -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff /dev/null ft
echo -----------------------------------------------
cat /dev/random | head -c 465879 > random
cat random | ./ft_ssl des3-ecb -i random -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3-ecb -k FEDCBA888 -v BBFFEE66 -d > ft
diff random ft
echo DECRYPTION DES-PCBC
echo -----------------------------------------------
./ft_ssl des-pcbc -i auteur -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-pcbc -k FEDCBA888 -v BBFFEE66 -d > ft
diff ft auteur
echo -----------------------------------------------
./ft_ssl des-pcbc -i /bin/ls -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-pcbc -k FEDCBA888 -v BBFFEE66 -d > ft
diff /bin/ls ft
echo -----------------------------------------------
./ft_ssl des-pcbc -i /dev/null -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-pcbc -k FEDCBA888 -v BBFFEE66 -d > ft
diff /dev/null ft
echo -----------------------------------------------
cat /dev/random | head -c 465879 > random
cat random | ./ft_ssl des3-pcbc -i random -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des-pcbc -k FEDCBA888 -v BBFFEE66 -d > ft
diff random ft
echo DECRYPTION DES3-PCBC
echo -----------------------------------------------
./ft_ssl des3-pcbc -i auteur -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3-pcbc -k FEDCBA888 -v BBFFEE66 -d > ft
diff ft auteur
echo -----------------------------------------------
./ft_ssl des3-pcbc -i /bin/ls -k FEDDD9CBA888 -v 1BBFFEE66A | ./ft_ssl des3-pcbc -k FEDDD9CBA888 -v 1BBFFEE66A -d > ft
diff /bin/ls ft
echo -----------------------------------------------
./ft_ssl des3-pcbc -i /dev/null -k FEDCBA888 -v BBFFEE66 | ./ft_ssl des3-pcbc -k FEDCBA888 -v BBFFEE66 -d > ft
diff /dev/null ft
echo -----------------------------------------------
cat /dev/random | head -c 465879 > random
cat random | ./ft_ssl des3-pcbc -i random -k DDFEDCBA888 -v BBFFEE86689 | ./ft_ssl des3-pcbc -k DDFEDCBA888 -v BBFFEE86689 -d > ft
diff random ft

rm random ft
echo -----------------------------------------------
echo -----------------------------------------------
echo -----------------------------------------------
