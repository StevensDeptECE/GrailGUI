// to encrypt/decrypt via cli
// openssl aes-256-cbc -a -salt -in secrets.txt -out secrets.txt.enc
// openssl aes-256-cbc -d -a -in secrets.txt.enc -out secrets.txt.new
// https://stackoverflow.com/questions/16056135/how-to-use-openssl-to-encrypt-decrypt-files

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string.h>
#include <iostream>
#include <sstream>

#define KEYLEN 32
// https://stackoverflow.com/questions/43192493/aes-256-cbc-encryption-c-using-openssl

int encrypt(unsigned char* plaintext, int plaintext_len, unsigned char *key, 
    unsigned char *iv, unsigned char *cipher) {

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    int len, cipher_len;

    int outlen1, outlen2;
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, cipher, &len, plaintext, plaintext_len);
    EVP_EncryptFinal_ex(ctx, cipher+len, &len);

    printf("%s", cipher);
    
    EVP_CIPHER_CTX_free(ctx);

    return cipher_len;
}

int main(int argc, char **argv) {

    unsigned char *key = argc > 1 ? 
        (unsigned char*) argv[1] 
        : (unsigned char*) "01234567890123456789012345678901";

    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));

    unsigned char *plaintext = (unsigned char*) "Secrets";

    unsigned char cipher[128];
    unsigned char decryp[128];

    int decrypt_len, cipher_len;
    cipher_len = encrypt(plaintext, strlen((char*) plaintext), 
        key, iv, cipher);

    printf("%s\n", (const char*) cipher);

    return 0;
}