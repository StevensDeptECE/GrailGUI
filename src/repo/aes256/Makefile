LIBS 	= -lssl -lcrypto
CFLAGS	= -g -o	

all: clean aes_enc_dec

# encrypts a file with AES256|
# uses EVP API for black-box approach to AES256 - encryption scheme can be easily swapped
aes_enc_dec: 
	g++ $(CFLAGS) aes_enc_dec aes_enc_dec.cc $(LIBS)    

aes_enc_file: 
	g++ $(CFLAGS) aes_enc_file aes_enc_file.cc $(LIBS)

openfile:
	g++ $(CFLAGS) openfile openfile.cc $(LIBS)

clean:
	rm -f openfile aes_enc_dec aes_enc_file