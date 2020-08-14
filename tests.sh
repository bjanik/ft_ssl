#!/bin/bash

set -e

## Generating private keys
echo "---------Generating private keys---------"

./ft_ssl genrsa -o key.pem1 $1
./ft_ssl genrsa -o key.pem2 -des -passout pass:cyberpunk $1
./ft_ssl genrsa -o key.des -des -passout pass:cyberpunk $1

sleep 2
## Generating public keys

echo "---------Generating public keys-----------"


./ft_ssl rsa -in key.pem1 -out key.pub1 -pubout
./ft_ssl rsa -in key.pem2 -passin pass:cyberpunk -out key.pub2 -pubout
sleep 2

echo "---------Checking keys-----------"

# Checking RSA keys without printing
openssl rsa -in key.pem1 -check -noout

./ft_ssl rsa -in key.pem1 -check -noout

# Need DES password for decryption
./ft_ssl rsa -in key.pem2 -check -noout -passin pass:cyberpunk
openssl rsa -in key.pem2 -check -noout -passin pass:cyberpunk

export PASSWD=cyberpunk
echo cyberpunk > passwd_file
./ft_ssl rsa -in key.pem2 -check -passin env:PASSWD -noout
./ft_ssl rsa -in key.pem2 -check -passin file:passwd_file -noout

echo -n cyberpunk | ./ft_ssl rsa -in key.pem2 -check -passin stdin -noout

echo -n cyberpunk | openssl rsa -in key.pem2 -check -passin stdin -noout

sleep 2

## Encryption
echo "---------Encrypt message-----------"
echo "Message to encrypt" > plain
./ft_ssl rsautl -in plain -inkey key.pub1 -out cipher1 -pubin

sleep 2

echo "---------Decrypt message-----------"
./ft_ssl rsautl -in cipher1 -inkey key.pem1 -out plain1 -decrypt
diff plain1 plain
