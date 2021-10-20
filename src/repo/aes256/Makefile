LIBS 	= -lssl -lcrypto
CFLAGS	= -g -o	

all: clean aes_enc_dec

# encrypts a file with AES256|
# uses EVP API for black-box approach to AES256 - encryption scheme can be easily swapped
aes_enc_dec: 
	g++ $(CFLAGS) aes_enc_dec aes_enc_dec.cc $(LIBS)    

clean:
	rm -f aes_enc_dec