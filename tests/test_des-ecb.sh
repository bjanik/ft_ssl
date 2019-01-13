#!/bin/sh
echo ----------------- 1 -------------------
openssl des-ecb -in /bin/ls -K  789789789 > open
./ft_ssl des-ecb -i /bin/ls -k 789789789 > ft
diff open ft
echo ------------------ 2 ------------------
openssl des-ecb -in /dev/null -K 789789789  > open
./ft_ssl des-ecb -i /dev/null -k 789789789 > ft
diff open ft
echo ------------------- 3 -----------------
openssl des-ecb -in /bin/cat -K 789789789 > open
./ft_ssl des-ecb -i /bin/cat -k 789789789 > ft
diff open ft
echo ------------------- 4 -----------------
echo ------------------- 5 -----------------
echo ------------------- 6 -----------------
echo ------------------- 7 -----------------
echo ------------------- 8 -----------------
echo ------------------- 9 -----------------
