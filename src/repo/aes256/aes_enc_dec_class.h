#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void close_files(std::ifstream *in, std::ofstream *out) {
  in->close();
  out->close();
}

/**
 * @brief An instance of AESEncDec is tied to a key, with which
 * files can be encrypted or decrypted as desired.
 */
class AESEncDec {
  static constexpr size_t KEYLEN = 32;
  static constexpr size_t BLOCKSIZE = 16;
  static constexpr size_t ITER_COUNT = 10000;
  unsigned char key[KEYLEN];
  unsigned char iv[BLOCKSIZE];

 public:
  AESEncDec(unsigned char *keybase);
  long int encrypt_file(const char *path, const char *out);
  long int decrypt_file(const char *path, const char *out);
};

AESEncDec::AESEncDec(unsigned char *keybase) {
  if (!(PKCS5_PBKDF2_HMAC_SHA1(
          (const char *)keybase, strlen((const char *)keybase),
          // nullptr, 0 because no salt; consider adding salts later
          nullptr, 0, ITER_COUNT, KEYLEN, key))) {
    std::cerr << "Invalid key base";
    throw std::invalid_argument("key base");
  }
}

long int AESEncDec::encrypt_file(const char *path, const char *out) {
  // reinitialize iv to avoid reuse
  if (!RAND_bytes(iv, BLOCKSIZE)) {
    std::cerr << "Failed to initialize IV";
    return -1;
  }

  // initialize/open file streams
  std::ifstream plaintext_file;
  std::ofstream ciphertext_file;
  plaintext_file.open(path, std::ios::binary);
  ciphertext_file.open(out, std::ios::binary | std::ios::trunc);

  // ensure files are open, exit otherwise
  if (!plaintext_file.is_open() || !ciphertext_file.is_open()) {
    std::cerr << "Failed to open file";
    // one of the files could be open, close both to be sure
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  // initialize encryption buffers
  unsigned char plaintext[BLOCKSIZE];
  unsigned char ciphertext[BLOCKSIZE + BLOCKSIZE];  // extra space for padding

  // initialize encryption context
  EVP_CIPHER_CTX *ctx;
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    std::cerr << "Failed to initialize cipher context";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }
  // reinitialize iv to avoid reuse
  if (!RAND_bytes(iv, BLOCKSIZE)) {
    std::cerr << "Failed to initialize IV";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  // set cipher/key/iv
  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
    std::cerr << "Failed to initialize encrypt";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

    // set IV len to 16
  if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, BLOCKSIZE, nullptr)) {
    std::cerr << "Failed to set IV length";
    ERR_print_errors_fp(stderr);
    close_files(&plaintext_file, &ciphertext_file);
  }

  if (1 != EVP_EncryptInit_ex(ctx, nullptr, nullptr, key, iv)) {
    std::cerr << "Failed to set key + iv";
    ERR_print_errors_fp(stderr);
    close_files(&plaintext_file, &ciphertext_file);
  }


  // for keeping track of result length
  int len;
  long int cipherlen = 0;
  std::streamsize bytes_read;

  // put iv into encrypted file
  ciphertext_file.write((char *)iv, BLOCKSIZE);
  if (!ciphertext_file.good()) {
    std::cerr << "Failed to retrieve IV";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  // read and encrypt a block at a time, write to file
  while (1) {
    plaintext_file.read((char *)plaintext, BLOCKSIZE);
    bytes_read = plaintext_file.gcount();
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, bytes_read)) {
      std::cerr << "Failed to continue encryption";
      close_files(&plaintext_file, &ciphertext_file);
      return -1;
    }
    ciphertext_file.write((char *)ciphertext, len);
    if (!ciphertext_file.good()) {
      std::cerr << "Failed to write to cipher";
      close_files(&plaintext_file, &ciphertext_file);
      return -1;
    }
    cipherlen += len;
    if (bytes_read < BLOCKSIZE) break;
  }

  // finalize encryption
  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext, &len)) {
    std::cerr << "Failed to finalize encrypt";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  cipherlen += len;

  // write final block if needed (potentially never happens in gcm?)
  ciphertext_file.write((char *)ciphertext, len);
  if (!ciphertext_file.good()) {
    std::cerr << "Failed to write to cipher";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  if (!plaintext_file.eof()) {
    std::cerr << "Failed to read entire plaintext";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  // get tag and attach it to the file
  unsigned char *tag = (unsigned char *) calloc(BLOCKSIZE, 1);
  if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, BLOCKSIZE, tag)) {
    ERR_print_errors_fp(stderr);
    std::cerr << "Failed to get GCM tag" << std::endl;
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }
  
  std::cout << "Writing tag " << (unsigned char *) tag << " at index " << ciphertext_file.tellp() << std::endl;
  // last 16 bytes are tag
  ciphertext_file.write((char *) tag, BLOCKSIZE); 
  if (!ciphertext_file.good()) {
    std::cerr << "Writing tag failed";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }

  // clean up
  close_files(&plaintext_file, &ciphertext_file);
  EVP_CIPHER_CTX_free(ctx);
  free(tag);
  return cipherlen;
}

long int AESEncDec::decrypt_file(const char *path, const char *out) {
  // open files for reading and writing
  std::ifstream ciphertext_file;
  std::ofstream plaintext_file;
  ciphertext_file.open(path, std::ios::binary);
  plaintext_file.open(out, std::ios::binary | std::ios::trunc);

  // if opening failed, exit
  if (!ciphertext_file.is_open() || !plaintext_file.is_open()) {
    std::cerr << "One of the files is already open";
    close_files(&ciphertext_file, &plaintext_file);
    return -1;
  }

  // initialize cipher context
  EVP_CIPHER_CTX *ctx;
  if (!(ctx = EVP_CIPHER_CTX_new())) {
    std::cerr << "Failed to initialize cipher context";
    close_files(&ciphertext_file, &plaintext_file);
    return -1;
  }

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr)) {
    std::cerr << "Failed to initialize decrypt";
    close_files(&ciphertext_file, &plaintext_file);
    return -1;
  }

  if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, BLOCKSIZE, nullptr)) {
    std::cerr << "Failed to set IV length";
    ERR_print_errors_fp(stderr);
    close_files(&ciphertext_file, &plaintext_file);
    return -1;
  }

  // RETRIEVE IV
  ciphertext_file.read((char *)iv, BLOCKSIZE);
  if (ciphertext_file.gcount() == 0) {
    std::cerr << "Failed to retrieve IV";
    close_files(&ciphertext_file, &plaintext_file);
    return -1;
  }

  if (1 != EVP_DecryptInit_ex(ctx, nullptr, nullptr, key, iv)) {
    std::cerr << "Failed to set key+iv" << std::endl;
    ERR_print_errors_fp(stderr);
    close_files(&ciphertext_file, &plaintext_file);
  }

  // get tag from last 16 bytes
  ciphertext_file.seekg(-BLOCKSIZE, std::ios_base::end);

  // get end of encrypted data to know where to end
  int end_encrypted = ciphertext_file.tellg();


  // unsigned char tag[BLOCKSIZE];
  unsigned char* tag = (unsigned char *) calloc(BLOCKSIZE, 1);
  ciphertext_file.read((char *) tag, BLOCKSIZE);
  std::cout << "Got tag " << (char *) tag << " from cipher" << std::endl;
  if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, BLOCKSIZE, tag)) {
    ERR_print_errors_fp(stderr);
  }
  // if (!ciphertext_file.eof()) {
  //   std::cerr << "Tag is not end of file; At location " << ciphertext_file.tellg() << std::endl;;
  //   close_files(&ciphertext_file, &plaintext_file);
  //   return -1;
  // }

  // keeping track of length of result
  int len;
  long int plaintext_len = 0;

  // initialize cipher/plaintext buffers
  unsigned char plaintext[BLOCKSIZE + BLOCKSIZE], ciphertext[BLOCKSIZE];

  std::streamsize bytes_read;

  // start after IV
  ciphertext_file.seekg(BLOCKSIZE, std::ios_base::beg);

  // go through the file one block at a time
  do {
    ciphertext_file.read((char *)ciphertext, BLOCKSIZE);
    bytes_read = ciphertext_file.gcount();

    // decrypt block
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, bytes_read)) {
      std::cerr << "Decrypt failed";
      close_files(&ciphertext_file, &plaintext_file);
      // Proper way to exit??
      return -1;
    }
    plaintext_len += len;

    // write decrypted result and ensure success
    plaintext_file.write((char *)plaintext, len);
    if (!plaintext_file.good()) {
      std::cerr << "Failed to write decryption";
      close_files(&ciphertext_file, &plaintext_file);
      return -1;
    }
    if (bytes_read < BLOCKSIZE) break;
  } while (ciphertext_file.tellg() < end_encrypted);

  // consider working on copies of file to avoid damaging user's content?
  if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) <= 0) {
    std::cerr << "Failed to decrypt or authenticate" << std::endl;
    ERR_print_errors_fp(stderr);
  }

  // // write & ensure success
  // plaintext_file.write((char *)plaintext, len);
  // if (!plaintext_file.good()) {
  //   std::cerr << "Writing to plaintext failed." << std::endl;
  //   close_files(&ciphertext_file, &plaintext_file);
  //   return -1;
  // }

  // plaintext_len += len;

  // if (!plaintext_file.good()) {
  //   std::cerr << "Failed to write to plaintext" << std::endl;
  //   plaintext_file.close();
  //   ciphertext_file.close();
  //   return -1;
  // }

  // clean up
  EVP_CIPHER_CTX_free(ctx);
  close_files(&ciphertext_file, &plaintext_file);
  free(tag);

  return plaintext_len;
}