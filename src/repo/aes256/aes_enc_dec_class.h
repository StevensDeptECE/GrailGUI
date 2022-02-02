#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <sstream>
#include <cstdio>

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
        long int encrypt_file(const char* path, 
            const char* out);
        long int decrypt_file(const char* path,
            const char* out);
};

AESEncDec::AESEncDec(unsigned char* keybase) {
    // set key from keybase
    if (!(EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), NULL,
        keybase, (int) strlen((const char *) keybase), ITER_COUNT, key, iv))) {
        std::cerr <<  "Invalid key base";
    }
}

long int AESEncDec::encrypt_file(const char* path, const char* out) {
    // initialize encryption context
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        std::cerr << "Failed to initialize cipher context.";
        return -1;
    }

    // reinitialize iv to avoid reuse
    if (!RAND_bytes(iv, BLOCKSIZE)) {
        std::cerr <<  "Failed to initialize IV";
        return -1;
    }

    // initialize/open file streams
    std::ifstream plaintext_file;
    std::ofstream ciphertext_file;
    plaintext_file.open(path, std::ios::binary);
    ciphertext_file.open(out, std::ios::binary | std::ios::trunc);

    // ensure files areopen, exit otherwise
    if (!plaintext_file.is_open() || !ciphertext_file.is_open()) {
        std::cerr <<  "Failed to open file";
        return -1;
    }

    // initialize encryption buffers
    unsigned char plaintext[BLOCKSIZE];
    unsigned char ciphertext[BLOCKSIZE + BLOCKSIZE]; // extra space for padding
    


    // set cipher/key/iv
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        plaintext_file.close();
        ciphertext_file.close();
        ERR_print_errors_fp(stderr);
        return -1;
    }

    // for keeping track of result length
    int len;
    long int cipherlen = 0;
    std::streamsize bytes_read;

    // read and encrypt a block at a time, write to file
    while (1) {
        plaintext_file.read((char *) plaintext, BLOCKSIZE);
        bytes_read = plaintext_file.gcount();
        if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, (int) bytes_read)) {
            ERR_print_errors_fp(stderr);
            return -1;
        }
        
        ciphertext_file.write((char *) ciphertext, len);
        if (!ciphertext_file.good()) {
            std::cerr << "Failed to write to ciphertext";
            plaintext_file.close();
            ciphertext_file.close();
        }

        cipherlen+=len;
        if (bytes_read < BLOCKSIZE) break;
    }

    // ensure file was read correctly
    if (!plaintext_file.eof()) {
        plaintext_file.close();
        ciphertext_file.close();
        std::cerr << "Failed to reach EOF";
        return -1;
    }

    // finalize encryption
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext, &len)) {
        std::cerr << "Failed to finalize encryption";
        ERR_print_errors_fp(stderr);
        return -1;
    }

    // write final block
    ciphertext_file.write((char *) ciphertext, len);
    cipherlen += bytes_read;

    // clean up
    ciphertext_file.close();
    plaintext_file.close();
    EVP_CIPHER_CTX_free(ctx);
    return cipherlen;
} 

long int AESEncDec::decrypt_file(const char* path, 
    const char* out) {
    
    // initialize cipher context
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        std::cerr << "Failed to initialize context";
        ERR_print_errors_fp(stderr);
        return -1;
    }

    // open files for reading and writing
    std::ifstream ciphertext_file;
    std::ofstream plaintext_file;
    ciphertext_file.open(path, std::ios::binary);
    plaintext_file.open(out, std::ios::binary | std::ios::trunc);

    // if opening failed, exit
    if (!ciphertext_file.is_open() || !plaintext_file.is_open()) {
        std::cerr <<  "One of the files is already open";
        return -1;
    }
    

    // initialize decryption 
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        std:cerr << "Failed to initialize decryption.";
        ERR_print_errors_fp(stderr);
        return -1;
    }
        // handleErrors();
    
    // keeping track of length of result
    int len;
    long int plaintext_len = 0;

    // initialize cipher/plaintext buffers
    unsigned char plaintext[BLOCKSIZE+BLOCKSIZE], ciphertext[BLOCKSIZE];    

    std::streamsize bytes_read;

    // go through the file one block at a time
    while (1) {
        ciphertext_file.read((char *) ciphertext, BLOCKSIZE);
        bytes_read = ciphertext_file.gcount();

        // decrypt block
        if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, (int) bytes_read)) {
            std::cerr << "Failed to decrypt";
            ERR_print_errors_fp(stderr);
            return -1;
        }
            
            handleErrors();
        
        plaintext_file.write((char *) plaintext, len);
        plaintext_len += len;
        
        // confirm write succeeded
        if (!plaintext_file.good()) {
            std::cerr << "Writing to plaintext failed";
            ciphertext_file.close();
            plaintext_file.close();
            return -1;
        }
        if (bytes_read < BLOCKSIZE) break;
    } 

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();

    if (!ciphertext_file.eof()) {
        std::cerr << "Failed to read full plaintext";
        ciphertext_file.close();
        plaintext_file.close();
        return -1;
    }

    plaintext_file.write((char*) plaintext, len);
    plaintext_len += len;

    if (!plaintext_file.good()) {
        std::cerr << "Failed to write to plaintext";
        plaintext_file.close();
        ciphertext_file.close();
        return -1;
    }

    // clean up
    EVP_CIPHER_CTX_free(ctx);
    plaintext_file.close();
    ciphertext_file.close();

    return plaintext_len;
}