<<<<<<< HEAD
# rewritten according to https://codereview.stackexchange.com/a/270140/251828
all: test_aes_enc_dec

CXX = g++
CXXFLAGS += -std=c++20
=======
all: test_aes_enc_dec

CXX = g++
>>>>>>> 2d1b5f9d7a788fb8e7d161cf6ab5a1fd59fa6505
CXXFLAGS += -g
CXXFLAGS += -Wall -Wextra -Wwrite-strings -Weffc++
CXXFLAGS += -Wpedantic -Warray-bounds -Wconversion
CXXFLAGS += -Wno-parentheses
CXXFLAGS += $(shell pkg-config --cflags openssl)
LINK.o = $(LINK.cc)
LDLIBS += $(shell pkg-config --libs openssl)

test_aes_enc_dec.o: aes_enc_dec_class.h

clean:
	$(RM) test_aes_enc_dec *.o test.*

.PHONY: clean