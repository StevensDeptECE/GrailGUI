// to encrypt/decrypt via cli
// openssl aes-256-cbc -a -salt -in secrets.txt -out secrets.txt.enc
// openssl aes-256-cbc -d -a -in secrets.txt.enc -out secrets.txt.new
// https://stackoverflow.com/questions/16056135/how-to-use-openssl-to-encrypt-decrypt-files

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <string.h>
#include <iostream>
#include <sstream>

#define KEYLEN 32

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

int encrypt(unsigned char* plaintext, int plaintext_len, unsigned char *key, 
    unsigned char *iv, unsigned char *cipher) {

    EVP_CIPHER_CTX *ctx;

    // initialize evp cipher context
    if (!(ctx = EVP_CIPHER_CTX_new())) 
        handleErrors();

    int len, cipher_len;

    int outlen1, outlen2;
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, cipher, &len, plaintext, plaintext_len))
        handleErrors();
    cipher_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, cipher+len, &len))
        handleErrors();
    cipher_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return cipher_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext) {
    
    EVP_CIPHER_CTX *ctx;

    // initializes evp context
    // todo: handle errors
    if (!(ctx = EVP_CIPHER_CTX_new())) 
        handleErrors();

    int len;
    int plaintxt_len;
    
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintxt_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintxt_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintxt_len;
}

// todo: pad key provided in argument
int main(int argc, char **argv) {

    unsigned char *plaintext = argc > 1 ? 
        (unsigned char *) argv[1]
        : (unsigned char*) "Secret message!";

    unsigned char *key = (unsigned char*) "01234567890123456789012345678901";

    unsigned char iv[16];
    if (!RAND_bytes(iv, sizeof(iv)))
        handleErrors();

    unsigned char cipher[128];
    unsigned char decryp[128];
    int decrypt_len, cipher_len;
    
    printf("Original message:\n%s\n", (const char *) plaintext);

    cipher_len = encrypt(plaintext, strlen((char*) plaintext), 
        key, iv, cipher);

    printf("Encrypted:\n");
    printf("%s\n", (const char*) cipher);

    decrypt_len = decrypt(cipher, cipher_len, key, iv, decryp);
    decryp[decrypt_len] = '\0';

    printf("Decrypted:\n%s\n", decryp);

    return 0;
}