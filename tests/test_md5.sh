#!/bin/bash
#set -e
./ft_ssl md5 Makefile > ft_ssl_result
md5 Makefile > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 ft_ssl  > ft_ssl_result
md5 ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -r ft_ssl  > ft_ssl_result
md5 -r ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -q ft_ssl '/bin/cat'  > ft_ssl_result
md5 -q ft_ssl '/bin/cat' > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
echo toto | ./ft_ssl md5 > ft_ssl_result
echo toto |  md5 > md5_result
diff md5_result ft_ssl_result
echo "------------- NEXT (output 'toto' twice) ---------------"
echo toto | ./ft_ssl md5 -p
echo toto |  md5 -p
echo ------------- NEXT ---------------
./ft_ssl md5 -s toto  > ft_ssl_result
md5 -s toto > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -s toto  > ft_ssl_result
md5 -s toto > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -rs toto  > ft_ssl_result
md5 -rs toto > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -q -rs toto ft_ssl  > ft_ssl_result
md5 -q -rs toto ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5  toto ft_ssl  > ft_ssl_result
md5  toto ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT  ---------------
./ft_ssl md5 -stoto -s toto  ft_ssl  > ft_ssl_result
md5 -stoto -s toto toto ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -s > ft_ssl_result
md5 -s > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -qqqqqs -rstoto -stoto -s ft_ssl  > ft_ssl_result
md5 -qqqqqs -rstoto -stoto -s  ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -q -rs toto ft_ssl  > ft_ssl_result
md5 -q -rs toto ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -q -rs toto ft_ssl  > ft_ssl_result
md5 -q -rs toto ft_ssl > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 "/bin/ls" > ft_ssl_result
md5 "/bin/ls" > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -- "/bin/ls" -- -s > ft_ssl_result
md5 --  "/bin/ls" -- -s > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
echo -n  'xxx' |./ft_ssl md5 -p -p > ft_ssl_result
echo -n 'xxx' | md5 -p -p  > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 '/dev/null' > ft_ssl_result
md5 '/dev/null' > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
cat 'ft_ssl' '/bin/ls' '/dev/null' | ./ft_ssl md5 -p > ft_ssl_result
cat 'ft_ssl' '/bin/ls' '/dev/null' | md5 -p > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -s toto -r -s toto -q Makfile Makefile > ft_ssl_result
md5 -s toto -r -s toto -q Makfile Makefile > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -s -s -s -s > ft_ssl_result
md5 -s -s -s -s > md5_result
diff md5_result ft_ssl_result
echo ------------- NEXT ---------------
./ft_ssl md5 -- --- > ft_ssl_result
md5 -- --- > md5_result
diff md5_result ft_ssl_result