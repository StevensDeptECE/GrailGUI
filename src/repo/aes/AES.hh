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
  long encrypt_file(const char *path, const char *out);
  long decrypt_file(const char *path, const char *out);
};
