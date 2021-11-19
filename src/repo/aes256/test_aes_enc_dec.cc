#include "aes_enc_dec_class.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

int main () {
    unsigned char *key = (unsigned char *) "HardcodedKey!";
    AESEncDec cipher(key);

    const char* test_file_path = "test.txt";
    // open test file for writing, clearing it
    std::ofstream test_file;
    test_file.open(test_file_path, std::ios::trunc);

    // put a test string into the file
    const char *test_string = "This is our test string!";
    const size_t test_len = strlen(test_string);
    test_file.write(test_string, test_len);
    test_file.close();

    // encrypt the file
    long int enc_result = cipher.encrypt_file("test.txt", "test.enc");
    printf("Encrypted %ld bytes.\n", enc_result);
    
    // decrypt the file 
    long int dec_result = cipher.decrypt_file("test.enc", "test.dec");
    std::cout << "Decrypted " << dec_result 
        << " byte" << (dec_result > 1 ? "s" : "") << std::endl;
    // check output contents
    std::ifstream result;
    result.open("test.dec", std::ios::binary);
    char *in = new char[test_len + 1];
    result.read(in, test_len);
    in[test_len] = '\0';
    std::cout << "Plaintext: " << test_string << std::endl 
        << "Decrypted plaintext: " << in << std::endl;
    
    // report results
    int diff;
    if ((diff = strcmp(test_string, in)) != 0)
        std::cout << "Test failed: difference " << diff << std::endl;
    else std::cout << "Test passed" << std::endl;

    return 0;
}