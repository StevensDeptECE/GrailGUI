# rewritten according to https://codereview.stackexchange.com/a/270140/251828
all: test_aes_enc_dec

CXX = g++
CXXFLAGS += -std=c++2a
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