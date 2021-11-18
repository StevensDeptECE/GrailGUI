#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <sstream>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

/**
 * @brief An instance of AESEncDec is tied to a key, with which
 * files can be encrypted or decrypted as desired.
 */
class AESEncDec {
        static constexpr int KEYLEN = 32;
        static constexpr int BLOCKSIZE = 16;
        static constexpr int ITER_COUNT = 10000;
        unsigned char key[KEYLEN];
        unsigned char iv[BLOCKSIZE];
    public:
        AESEncDec(unsigned char* keybase);
        int encrypt_file(const char* path, 
            const char* out);
        int decrypt_file(const char* path,
            const char* out);
};

AESEncDec::AESEncDec(unsigned char* keybase) {
    // set key from keybase
    if (!(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), NULL,
        keybase, strlen((const char *) keybase), ITER_COUNT, key, iv))) {
        std::cerr <<  "Invalid key base";
    }
}

int AESEncDec::encrypt_file(const char* path, const char* out) {
    // initialize/open file streams
    std::ifstream plaintext_file;
    std::ofstream ciphertext_file;
    plaintext_file.open(path, std::ios::in | std::ios::binary);
    ciphertext_file.open(out, std::ios::out | std::ios::binary | std::ios::trunc);

    // ensure file is open, exit otherwise
    if (!plaintext_file.is_open()) {
        std::cerr <<  "Failed to open plaintext";
        return -1;
    }

    // initialize encryption buffers
    unsigned char plaintext[BLOCKSIZE];
    unsigned char ciphertext[BLOCKSIZE + BLOCKSIZE]; // extra space for padding
    
    // initialize encryption context
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // reinitialize iv to avoid reuse
    if (!RAND_bytes(iv, BLOCKSIZE)) {
        std::cerr <<  "Failed to initialize IV";
        return -1;
    }

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

int AESEncDec::decrypt_file(const char* path, 
    const char* out) {
    
    // open files for reading and writing
    std::ifstream ciphertext_file;
    std::ofstream plaintext_file;
    ciphertext_file.open(path, std::ios::in | std::ios::binary);
    plaintext_file.open(out, std::ios::out | std::ios::binary | std::ios::trunc);

    // if opening failed, exit
    if (!ciphertext_file.is_open() || !plaintext_file.is_open()) {
        std::cerr <<  "One of the files is already open";
        return -1;
    }
    
    // initialize cipher context
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    // reinitialize iv to avoid reuse
    // if (!RAND_bytes(iv, BLOCKSIZE)) {
    //     std::cerr <<  "Failed to initialize IV");
    //     return -1;
    // }

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

        // decrypt block
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
    
    // clean up
    EVP_CIPHER_CTX_free(ctx);
    plaintext_file.close();
    ciphertext_file.close();

    return plaintext_len;
}