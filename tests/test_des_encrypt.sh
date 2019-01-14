#!/bin/sh
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
echo ---------------------------------------------
openssl des-ecb -in /bin/bash -K 789789789 > open
./ft_ssl des-ecb -i /bin/bash -k 789789789 > ft
diff open ft
echo ---------------------------------------------
openssl des-ecb -in /usr/bin/openssl -K 789789789 > open
./ft_ssl des-ecb -i /usr/bin/openssl -k 789789789 > ft
diff open ft
echo ---------------- DES-CBC --------------------
openssl des -in /bin/ls -K  789789789 -iv CCD4588 > open
./ft_ssl des -i /bin/ls -k 789789789 -v CCD4588 > ft
diff open ft
echo ---------------------------------------------
openssl des -in /dev/null -K 789789789 -iv CCD4588 > open
./ft_ssl des -i /dev/null -k 789789789 -v CCD4588 > ft
diff open ft
echo ---------------------------------------------
openssl des -in /bin/cat -K 789789789 -iv CCD4588 > open
./ft_ssl des -i /bin/cat -k 789789789 -v CCD4588 > ft
diff open ft
echo ---------------------------------------------
openssl des-ecb -in /usr/bin/openssl -K 789789789 -iv CCD4588 > open
./ft_ssl des-ecb -i /usr/bin/openssl -k 789789789  -v CCD4588 > ft
diff open ft

rm open ft