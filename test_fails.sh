#!/bin/bash

./ft_ssl abc
sleep 1

./ft_ssl genrsa -out
sleep 1

./ft_ssl genrsa -in
sleep 1


./ft_ssl genrsa -o
sleep 1

./ft_ssl genrsa -o key.pem -des -passout
sleep 1


./ft_ssl rsa -in
sleep 1

./ft_ssl rsa -out
sleep 1

./ft_ssl rsa -inform
sleep 1

./ft_ssl rsa -inform DER
sleep 1

./ft_ssl rsa -outform
sleep 1

./ft_ssl rsa -outform DER
sleep 1

./ft_ssl rsa -in key.pem -passin

./ft_ssl genrsa -o key.pem 64
./ft_ssl rsa -in key.pem -pubin
sleep 1

./ft_ssl genrsa -o key.pem -des -passout pass:cyber 256 # Gen encrypted key with password
./ft_ssl rsa -in key.pem -passin pass:toto

export TOTO=toto
echo toto > toto
./ft_ssl rsa -in key.pem -passin pass:toto
./ft_ssl rsa -in key.pem -passin env:TOTO
./ft_ssl rsa -in key.pem -passin file:toto
./ft_ssl rsa -in key.pem -passin pass:
./ft_ssl rsa -in key.pem -passin env:
./ft_ssl rsa -in key.pem -passin file:

./ft_ssl rsa -in key.pem -passin pass
./ft_ssl rsa -in key.pem -passin env
./ft_ssl rsa -in key.pem -passin file

./ft_ssl rsa -in key.pem -passin pa:
./ft_ssl rsa -in key.pem -passin en
./ft_ssl rsa -in key.pem -passin fil


export CYBER=cyber
./ft_ssl rsa -in key.pem -passin env:CYBER

./ft_ssl rsautl -inkey
./ft_ssl rsautl -in
./ft_ssl rsautl -out
./ft_ssl rsautl -decrypt -encrypt

