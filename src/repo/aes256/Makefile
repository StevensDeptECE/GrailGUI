LIBS 	= -lssl -lcrypto
CFLAGS	= -g -o	

all: aes_encrypt_file

# encrypts a file given an AES-256 key and IV
aes_encrypt_file: 
	g++ $(CFLAGS) aes_encrypt_file aes256_encrypt_file.cc $(LIBS)    

# generates an AES-256 key and IV with openssl/rand.h function, puts both to be sent to stdout
# the plan is to pipe the keys to a file or the encryption program 
aes_key_IV:
	gcc $(CFLAGS) aes256_key_IV aes256_key_IV.cc $(LIBS)

clean:
	rm -f aes_encrypt_file aes256_key_IV