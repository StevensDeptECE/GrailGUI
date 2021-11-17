# Old files associated with the AES256 section of the repo
# May be useful for anyone looking to work with AES

LIBS 	= -lssl -lcrypto
CFLAGS	= -g -o	

all: clean aes_enc_dec aes_enc_dec_str

# Functional module that encrypts and decrypts files
aes_enc_dec: 
	g++ $(CFLAGS) aes_enc_dec aes_enc_dec.cc $(LIBS)    

# Small example of encrypting a string using AES256
aes_enc_dec_str:
	g++ $(CFLAGS) aes_enc_dec_str aes_enc_dec_str.cc $(LIBS)