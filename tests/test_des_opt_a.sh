#!/bin/sh
# set -e
echo ---------- TEST DES-ECB ----------
./ft_ssl des-ecb -i auteur -k 778855 -a |  tr -d '\n' > 42
openssl des-ecb -in auteur -K 778855 -a |  tr -d '\n'  > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/ls -k 778855 -a |  tr -d '\n'> 42
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
./ft_ssl des-ecb -i ft_ssl -k 778855000DDDF -a |  tr -d '\n'> 42
openssl des-ecb -in ft_ssl -K 778855000DDDF -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i ft_ssl -k 778855000DDDF -a |  tr -d '\n'> 42
openssl des-ecb -in ft_ssl -K 778855000DDDF -a | tr -d '\n' > 43
diff 42 43

echo ---------- TEST DES-CBC ----------
./ft_ssl des-ecb -i auteur -k 77008855 -v 78DFEDAC456 -a |  tr -d '\n'> 42
openssl des-ecb -in auteur -K 77008855 -iv 78DFEDAC456 -a |  tr -d '\n'  > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/ls -k 7788DDFF55 -v 78DFEDAC456 -a |  tr -d '\n'> 42
openssl des-ecb -in /bin/ls -K 7788DDFF55 -iv 78DFEDAC456 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/bash -k 7788ADFC55 -v 78DFEDAC456 -a |  tr -d '\n'> 42
openssl des-ecb -in /bin/bash -K 7788ADFC55 -iv 78DFEDAC456 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i /bin/cat -k 778855788 -v 78DFEDAC456 -a |  tr -d '\n'> 42
openssl des-ecb -in /bin/cat -K 778855788 -iv 78DFEDAC456 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des-ecb -i ft_ssl -k 778855000DDDF -v 78DFEDAC456 -a |  tr -d '\n'> 42
openssl des-ecb -in ft_ssl -K 778855000DDDF -iv 78DFEDAC456 -a | tr -d '\n' > 43
diff 42 43

echo ---------- TEST DES3-CBC ----------
./ft_ssl des3 -i auteur -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -v 45DEFACB -a |  tr -d '\n'> 42
openssl des3 -in auteur -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -iv 45DEFACB -a |  tr -d '\n'  > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-cbc -i /bin/ls -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -v 45DEFACB -a |  tr -d '\n'> 42
openssl des3 -in /bin/ls -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -iv 45DEFACB -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-cbc -i /bin/bash -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -v 45DEFACB -a |  tr -d '\n'> 42
openssl des3 -in /bin/bash -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -iv 45DEFACB -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-cbc -i /bin/cat -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -v 45DEFACB -a |  tr -d '\n'> 42
openssl des3 -in /bin/cat -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -iv 45DEFACB -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-cbc -i ft_ssl -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -v 45DEFACB -a |  tr -d '\n'> 42
openssl des3 -in ft_ssl -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -iv 45DEFACB -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-cbc -i /usr/bin/openssl -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -v 45DEFACB -a |  tr -d '\n'> 42
openssl des3 -in /usr/bin/openssl -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -iv 45DEFACB -a | tr -d '\n' > 43
diff 42 43

echo ---------- TEST DES3-ECB ----------
./ft_ssl des3-ecb -i auteur -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a |  tr -d '\n'> 42
openssl des-ede3 -in auteur -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a |  tr -d '\n'  > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-ecb -i /bin/ls -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a |  tr -d '\n'> 42
openssl des-ede3 -in /bin/ls -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-ecb -i /bin/bash -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a |  tr -d '\n'> 42
openssl des-ede3 -in /bin/bash -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-ecb -i /bin/cat -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a |  tr -d '\n'> 42
openssl des-ede3 -in /bin/cat -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-ecb -i ft_ssl -k 778855AABBA874CDFEEE500EDAC068464677ADFFDCC00 -a |  tr -d '\n'> 42
openssl des-ede3 -in ft_ssl -K 778855AABBA874CDFEEE500EDAC068464677ADFFDCC00 -a | tr -d '\n' > 43
diff 42 43
echo ----------------------------------
./ft_ssl des3-ecb -i /usr/bin/openssl -k 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a |  tr -d '\n'> 42
openssl des-ede3 -in /usr/bin/openssl -K 778855AAAAA874CDFEEE500EDAC068464696ADFFFCC00 -a | tr -d '\n' > 43
diff 42 43

rm 42 43