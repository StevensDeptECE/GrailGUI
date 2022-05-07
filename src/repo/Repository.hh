#pragma once
#include <cstdint>
#include <utility>
#include "memory.h"

class Blob {
private:
	uint8_t* p;
	uint32_t len;
public:
	Blob(const char* s) : len(strlen(s)) {
		p = new uint8_t[len];
		memcpy(p, s, len);
	}
	Blob(const uint8_t* v, uint32_t len) : p(new uint8_t[len]), len(len) {
		memcpy(p, v, len);
	}
	~Blob() {
		delete [] p;
	}
  Blob(const Blob& orig) : p(new uint8_t[orig.len]), len(orig.len) {
		memcpy(p, orig.p, len);
	}

	Blob& operator =(Blob orig) {
		len = orig.len;
		std::swap(p, orig.p);
		return *this;
	}
	//https://en.wikipedia.org/wiki/Six-bit_character_code
	friend std::ostream& operator <<(std::ostream& s, const Blob& b);
};

class Repository {
private:
	Blob key; // key.length() key.size()
};
	
