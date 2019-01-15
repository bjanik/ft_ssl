#!/bin/sh
set -e
echo ----------------- DES-ECB -------------------
openssl des-ecb -in /bin/ls -K  789789789 > open
./ft_ssl des-ecb -i /bin/ls -k 789789789 > ft
diff open ft
echo ---------------------------------------------
openssl des-ecb -in /dev/null -K 789789789  > open
./ft_ssl des-ecb -i /dev/null -k 789789789 > ft
diff open ft
echo ---------------------------------------------
openssl des-ecb -in /bin/cat -K 789789789 > open
./ft_ssl des-ecb -i /bin/cat -k 789789789 > ft
diff open ft