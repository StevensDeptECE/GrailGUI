#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>

struct key_iv {
    unsigned char key[32];
    unsigned char iv[32];
};

int generate_key_and_iv(key_iv *buf) {

    if (!RAND_bytes(buf->key, sizeof buf->key)) {
        fprintf(stderr, "Error: keygen failed\n");
        return -1;
    }
    if (!RAND_bytes(buf->iv, sizeof buf->iv)) {
        fprintf(stderr, "Error: IV gen failed\n");
        return -1;
    }

    return 0;
}

int main() {
    key_iv keypair;

    if (generate_key_and_iv(&keypair) != 0) {
        fprintf(stderr, "Error.");
        return -1;
    }

    fprintf(stdout, "KEY: %s\nIV: %s\n", keypair.key, keypair.iv);

    return 0;
}
