#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <sstream>

#define KEYLEN 32
#define BLOCKSIZE 16
#define ITER_COUNT 10000

int encrypt_file(const char *path, const char* out, unsigned char *key, unsigned char *iv);
int decrypt_file(const char *path, const char* out, unsigned char *key, unsigned char *iv);
// TODO: figure out correct error reporting
void handleErrors();

// usage: ./aes_enc_dec <filename> <key> <mode> <output>
int main(int argc, char *argv[]) {

    // set input filename
    const char *filename = argc > 1 ? 
        argv[1]
        : "secrets.enc";

    // set 32-bit key
    unsigned char *keybase = argc > 2 ? 
        (unsigned char *) argv[2] 
        : (unsigned char *) "01234567890123456789012345678901";
    
    unsigned char key[KEYLEN];
    unsigned char iv[BLOCKSIZE];

    // generate the key
    if (!(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), NULL,
        keybase, strlen((const char *) keybase), ITER_COUNT, key, iv))) {
            fprintf(stderr, "Invalid key.\n");
            return -1;
        }

    // mode 'e' vs 'd'; encrypt vs decrypt
    char mode = argc > 3 ? argv[3][0] : 'd';

    // set output file
    const char *out = argc > 4 ?
        argv[4]
        : "secrets.dec";

    int result;

    // e encrypts, d decrypts
    switch (mode) {
        case 'e':
            printf("Encrypting file...\n"); 
            result = encrypt_file(filename, out, key, iv);
            printf("Bytes encrypted: %d\n", result);
            break;
        case 'd':
            printf("Decrypting file...\n");
            result = decrypt_file(filename, out, key, iv);
            printf("Bytes decrypted: %d\n", result);
            break;
    };

    return 0;
}

int encrypt_file(const char* path, const char *out, unsigned char *key, unsigned char *iv) {

    // initialize/open file streams
    std::ifstream plaintext_file;
    std::ofstream ciphertext_file;
    plaintext_file.open(path, std::ios::in | std::ios::binary);
    ciphertext_file.open(out, std::ios::out | std::ios::binary);

    // ensure file is open, exit otherwise
    if (!plaintext_file.is_open()) {
        fprintf(stderr, "Failed to open plaintext.\n");
        return -1;
    }

    // initialize encryption buffers
    unsigned char plaintext[BLOCKSIZE];
    unsigned char ciphertext[BLOCKSIZE + BLOCKSIZE]; // extra space for padding
    
    // initialize encryption context
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // set cipher/key/iv
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))     
        handleErrors();

    // for keeping track of result length
    int len;
    int cipherlen = 0;
    int bytes_read;


    // read and encrypt a block at a time, write to file
    while (1) {
        plaintext_file.read((char *) plaintext, BLOCKSIZE);
        bytes_read = plaintext_file.gcount();
        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, bytes_read))
            handleErrors();
        
        ciphertext_file.write((char *) ciphertext, len);
        cipherlen+=len;
        if (bytes_read < BLOCKSIZE) break;
    }

    // finalize encryption
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext, &len))
        handleErrors();

    // write final block
    ciphertext_file.write((char *) ciphertext, len);
    cipherlen += bytes_read;

    // clean up
    ciphertext_file.close();
    plaintext_file.close();
    EVP_CIPHER_CTX_free(ctx);

    return cipherlen;

}

int decrypt_file(const char *path, const char* out, unsigned char* key, unsigned char *iv) {
    std::ifstream ciphertext_file;
    std::ofstream plaintext_file;

    // open files for reading and writing
    ciphertext_file.open(path, std::ios::in | std::ios::binary);
    plaintext_file.open(out, std::ios::out | std::ios::binary);

    // if opening failed, exit
    if (!ciphertext_file.is_open() || !plaintext_file.is_open()) {
        fprintf(stderr, "One of the files is already open.\n");
        return -1;
    }
    
    // initialize cipher context
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // initialize decryption 
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();
    
    // keeping track of length of result
    int len;
    int plaintext_len = 0;

    // initialize cipher/plaintext buffers
    unsigned char plaintext[BLOCKSIZE+BLOCKSIZE], ciphertext[BLOCKSIZE];    

    int bytes_read;

    // go through the file one block at a time
    while (1) {
        ciphertext_file.read((char *) ciphertext, BLOCKSIZE);
        bytes_read = ciphertext_file.gcount();

        /** DECRYPT **/
        if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, bytes_read))
            handleErrors();
        plaintext_len += len;
        plaintext_file.write((char *) plaintext, len);
        if (bytes_read < BLOCKSIZE) break;        
    } 

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();

    plaintext_file.write((char*) plaintext, len);
    plaintext_len += len;
    
    EVP_CIPHER_CTX_free(ctx);

    plaintext_file.close();
    ciphertext_file.close();

    return plaintext_len;

     
}

