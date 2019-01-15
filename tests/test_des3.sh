#!/bin/sh
set -e
echo -------------TEST DES3-CBC---------------
openssl des3 -in auteur -K ADF1257FEC694AC147BFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3 -i auteur -k ADF1257FEC694AC147BFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des3 -in ft_ssl -K ADF1257FEC69447BFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3 -i ft_ssl -k ADF1257FEC69447BFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des3 -in /bin/ls -K ADF1257FEC694AC147BFE7800DA -iv 58CDE91A021B > open
./ft_ssl des3 -i /bin/ls -k ADF1257FEC694AC147BFE7800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des3 -in auteur -K A257FEC694ACBFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3 -i auteur -k A257FEC694ACBFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des3 -in auteur -K 1257F694AC147BFE7DC68DA -iv 58CDE91A0B > open
./ft_ssl des3 -i auteur -k 1257F694AC147BFE7DC68DA -v 58CDE91A0B > ft
diff ft open
echo -----------------------------------------
openssl des3 -in auteur -K ADF1888257FEC694AC147BFE7DC6800DA7776600 -iv 58CD1A021B > open
./ft_ssl des3 -i auteur -k ADF1888257FEC694AC147BFE7DC6800DA7776600 -v 58CD1A021B > ft
diff ft open
echo -----------------------------------------
openssl des3 -in auteur -K ADF1257FEC694AC147BFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3 -i auteur -k ADF1257FEC694AC147BFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
./ft_ssl des3 -i auteur -k ADF1257FEC694AC147BFE7DC6800DA55555 -v 58CDE91A021B | ./ft_ssl des3 -d -k ADF1257FEC694AC147BFE7DC6800DA55555 -v 58CDE91A021B  > ft
openssl des3 -in auteur -K ADF1257FEC694AC147BFE7DC6800DA55555 -iv 58CDE91A021B  | openssl des3 -d -K ADF1257FEC694AC147BFE7DC6800DA55555 -iv 58CDE91A021B > open
diff ft open
echo -----------------------------------------
./ft_ssl des3 -i auteur -k ADF1EEFFFAC8899AD -v 58CDE91A0 | ./ft_ssl des3 -d -k ADF1EEFFFAC8899AD -v 58CDE91A0 > ft
openssl des3 -in auteur -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 | openssl des3 -d -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 > open
diff ft open
echo -----------------------------------------
./ft_ssl des3 -i /dev/null -k ADF1EEFFFAC8899AD -v 58CDE91A0 | ./ft_ssl des3 -d -k ADF1EEFFFAC8899AD -v 58CDE91A0 > ft
openssl des3 -in /dev/null -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 | openssl des3 -d -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 > open
diff ft open
echo -----------------------------------------
./ft_ssl des3 -i /usr/bin/openssl -k ADF1EEFFFAC8899AD -v 58CDE91A0 | ./ft_ssl des3 -d -k ADF1EEFFFAC8899AD -v 58CDE91A0 > ft
openssl des3 -in /usr/bin/openssl -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 | openssl des3 -d -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 > open
diff ft open
echo -----------------------------------------
./ft_ssl des3 -i /bin/bash -k ADF1EEFFFAC8899AD -v 58CDE91A0 | ./ft_ssl des3 -d -k ADF1EEFFFAC8899AD -v 58CDE91A0 > ft
openssl des3 -in /bin/bash -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 | openssl des3 -d -K ADF1EEFFFFAC8899AD -iv 58CDE91A0 > open
diff ft open


echo -------------TEST DES3-ECB---------------
openssl des-ede3 -in auteur -K ADF1257FEC694AC147BFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3-ecb -i auteur -k ADF1257FEC694AC147BFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des-ede3 -in ft_ssl -K ADF1257FEC69447BFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3-ecb -i ft_ssl -k ADF1257FEC69447BFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des-ede3 -in /bin/ls -K ADF1257FEC694AC147BFE7800DA -iv 58CDE91A021B > open
./ft_ssl des3-ecb -i /bin/ls -k ADF1257FEC694AC147BFE7800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des-ede3 -in auteur -K A257FEC694ACBFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3-ecb -i auteur -k A257FEC694ACBFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
openssl des-ede3 -in auteur -K 1257F694AC147BFE7DC68DA -iv 58CDE91A0B > open
./ft_ssl des3-ecb -i auteur -k 1257F694AC147BFE7DC68DA -v 58CDE91A0B > ft
diff ft open
echo -----------------------------------------
openssl des-ede3 -in auteur -K ADF1888257FEC694AC147BFE7DC6800DA7776600 -iv 58CD1A021B > open
./ft_ssl des3-ecb -i auteur -k ADF1888257FEC694AC147BFE7DC6800DA7776600 -v 58CD1A021B > ft
diff ft open
echo -----------------------------------------
openssl des-ede3 -in auteur -K ADF1257FEC694AC147BFE7DC6800DA -iv 58CDE91A021B > open
./ft_ssl des3-ecb -i auteur -k ADF1257FEC694AC147BFE7DC6800DA -v 58CDE91A021B > ft
diff ft open
echo -----------------------------------------
./ft_ssl des3-ecb -i auteur -k  ADF1257FEC694AC147BFE7DC6800DA -v  58CDE91A021B | ./ft_ssl des3-ecb -d -k ADF1257FEC694AC147BFE7DC6800DA -v  58CDE91A021B > ft
openssl des-ede3 -in auteur -K ADF1257FEC694AC147BFE7DC6800DA -iv 58CDE91A021B | openssl des-ede3 -d -K  ADF1257FEC694AC147BFE7DC6800DA -iv 58CDE91A021B > open
diff ft open
echo -----------------------------------------
./ft_ssl des3-ecb -i auteur -k ADF1EEFFFFAC8899AD00CDF88 -v 58CDE91A0 | ./ft_ssl des3-ecb -d -k ADF1EEFFFFAC8899AD00CDF88 -v  58CDE91A0 > ft
openssl des-ede3 -in auteur -K ADF1EEFFFFAC8899AD00CDF88 -iv 58CDE91A0 | openssl des-ede3 -d -K ADF1EEFFFFAC8899AD00CDF88 -iv 58CDE91A0 > open
diff ft open


echo -------------TEST DES3-BC---------------
./ft_ssl des3-bc -i auteur -k 789ADEFADFE82C45FA98C878987EF7AD5E8 -v FE78CAD690 | ./ft_ssl des3-bc -k 789ADEFADFE82C45FA98C878987EF7AD5E8 -v FE78CAD690 -d > ft
diff ft auteur

echo -------------TEST DES3-PCBC---------------
./ft_ssl des3-pcbc -i auteur -k 789ADEFADFE82C45FA98C878987EF7AD5E8 -v FE78CAD690 | ./ft_ssl des3-pcbc -k 789ADEFADFE82C45FA98C878987EF7AD5E8 -v FE78CAD690 -d > ft
diff ft auteur

rm ft open