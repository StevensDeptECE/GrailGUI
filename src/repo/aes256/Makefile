LIBS 	= -lssl -lcrypto
CFLAGS	= -g -o	

all: clean aes_enc_dec

# 	TODO:
#  Publish for code review
#  Implement Triple-AES
#  Implement better key generation method
#  Convert into a class  

# encrypts/decrypts a file with AES256
# usage: 
# ./aes_enc_dec <input filename> <key> <mode (e/d)> <output filename> 
aes_enc_dec: 
	g++ $(CFLAGS) aes_enc_dec aes_enc_dec.cc $(LIBS)    

clean:
	rm -f openfile aes_enc_dec 

