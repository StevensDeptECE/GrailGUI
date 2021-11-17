#include "aes_enc_dec_class.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

int main (void) {
    unsigned char *key = (unsigned char *) "HardcodedKey!";
    AESEncDec cipher(key);

    const char* test_file_path = "test.txt";
    // open test file for writing, clearing it
    std::ofstream test_file;
    test_file.open(test_file_path, std::ios::out | std::ios::trunc);

    // put a test string into the file
    const char *test_string = "This is our test string!";
    const int test_len = strlen(test_string);
    test_file.write(test_string, test_len);
    test_file.close();

    // encrypt the file
    int enc_result = cipher.encrypt_file("test.txt", "test.enc");
    printf("Encrypted %d bytes.\n", enc_result);
    
    // decrypt the file 
    int dec_result = cipher.decrypt_file("test.enc", "test.dec");
    printf("Decrypted %d bytes.\n", dec_result);

    // check output contents
    std::ifstream result;
    result.open("test.dec", std::ios::in | std::ios::binary);
    char in[test_len + 1];
    result.read(in, test_len);
    in[test_len] = '\0';
    printf("Plaintext: %s\n"
        "Decrypted plaintext: %s\n", test_string, in);
    
    // report results
    int diff;
    if ((diff = strcmp(test_string, in)) != 0)
        printf("Test failed: difference %d\n", diff);
    else printf("Test passed.\n");
}