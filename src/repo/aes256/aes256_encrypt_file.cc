// to encrypt/decrypt via cli
// openssl aes-256-cbc -a -salt -in secrets.txt -out secrets.txt.enc
// openssl aes-256-cbc -d -a -in secrets.txt.enc -out secrets.txt.new
// https://stackoverflow.com/questions/16056135/how-to-use-openssl-to-encrypt-decrypt-files

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <string.h>
#include <iostream>

int aes_encrypt_file()

int main() {

    return "hey";
}