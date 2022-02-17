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
  AESEncDec(std::string keybase);
  long int encrypt_file(const char *path, const char *out);
  long int decrypt_file(const char *path, const char *out);
};

AESEncDec::AESEncDec(std::string keybase) {
  if (!(PKCS5_PBKDF2_HMAC_SHA1(
          keybase.c_str(), sizeof(keybase.c_str()),
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
  // initialize iv 
  if (!RAND_bytes(iv, BLOCKSIZE)) {
    std::cerr << "Failed to initialize IV";
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }


  // put iv into encrypted file
  ciphertext_file.write((char *)iv, BLOCKSIZE);
  if (!ciphertext_file.good()) {
    std::cerr << "Failed to retrieve IV";
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

  if (1 != EVP_CIPHER_CTX_set_padding(ctx, 0)) {
    std::cerr << "Failed to disable padding" << std::endl;
    ERR_print_errors_fp(stderr);
    close_files(&plaintext_file, &ciphertext_file);
  }

  // get length of file and move position back to start
  plaintext_file.seekg(0, std::ios_base::end);
  int eof = (int) plaintext_file.tellg();
  plaintext_file.seekg(0, std::ios_base::beg);

  // for keeping track of result length
  int len;
  long int cipherlen = 0;
  int bytes_read;
  int bytes_to_read;


  // read and encrypt a block at a time, write to file
  while (true) {
    bytes_to_read = std::min((int) (eof - plaintext_file.tellg()), (int) BLOCKSIZE);
    if (bytes_to_read <= 0) break;

    plaintext_file.read((char *)plaintext, bytes_to_read);
    bytes_read = (int) plaintext_file.gcount();

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

  // get tag and attach it to the file
  unsigned char tag[BLOCKSIZE];
  if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, BLOCKSIZE, tag)) {
    ERR_print_errors_fp(stderr);
    std::cerr << "Failed to get GCM tag" << std::endl;
    close_files(&plaintext_file, &ciphertext_file);
    return -1;
  }
  
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
  int end_encrypted = (int) ciphertext_file.tellg();

  // unsigned char tag[BLOCKSIZE];
  unsigned char* tag = (unsigned char *) calloc(BLOCKSIZE, 1);
  ciphertext_file.read((char *) tag, BLOCKSIZE);
  
  if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag)) {
    std::cerr << "Failed to set tag" << std::endl;
    ERR_print_errors_fp(stderr);
    close_files(&ciphertext_file, &plaintext_file);
  }

  // initialize encryption
  int len;
  long int plaintext_len = 0;
  unsigned char plaintext[BLOCKSIZE + BLOCKSIZE],
    ciphertext[BLOCKSIZE];
  int bytes_to_read;
  int bytes_read;

  // start after IV
  ciphertext_file.seekg(BLOCKSIZE, std::ios_base::beg);

  // go through the file one block at a time
  while (true) {


    bytes_to_read = std::min((int) (end_encrypted - ciphertext_file.tellg()), (int) BLOCKSIZE);
    if (bytes_to_read <= 0) break;

    ciphertext_file.read((char *)ciphertext, bytes_to_read);
    bytes_read = (int) ciphertext_file.gcount();

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
  }

  if (!plaintext_file.good()) {
    std::cerr << "Failed to write to plaintext" << std::endl;
    close_files(&ciphertext_file, &plaintext_file);

    return -1;
  }

  // consider working on copies of file to avoid damaging user's content?
  if (1 != EVP_DecryptFinal(ctx, plaintext + len + bytes_to_read, &len)) {
    std::cerr << "Failed to decrypt or authenticate" << std::endl;
    ERR_print_errors_fp(stderr);
    close_files(&ciphertext_file, &plaintext_file);

  } else {
    std::cout << "Authenticated!" << std::endl;
  }

  // clean up
  EVP_CIPHER_CTX_free(ctx);
  close_files(&ciphertext_file, &plaintext_file);

  return plaintext_len;
}