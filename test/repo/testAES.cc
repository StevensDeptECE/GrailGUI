#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "repo/aes/AES.hh"

void print_help_msg() { std::cout << "Usage: ./test_aes key" << std::endl; }

int main(int argc, char* argv[]) {
  std::string key;
  if (argc == 2) {
    key = argv[1];
  } else {
    print_help_msg();
  }

  AESEncDec cipher(key);

  // encrypt the file
  long int enc_result = cipher.encrypt_file("top_secret.pdf", "top_secret.enc");
  printf("Encrypted %ld bytes.\n", enc_result);

  // decrypt the file
  long int dec_result =
      cipher.decrypt_file("top_secret.enc", "top_secret_dec.pdf");
  std::cout << "Decrypted " << dec_result << " bytes." << std::endl;

  return 0;
}
