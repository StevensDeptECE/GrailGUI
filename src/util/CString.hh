#pragma once
#include <memory.h>

#include <iostream>
/*
        constant-size string, lighter weight
*/
class CString {
 private:
  uint32_t len_;
  char* p;

 public:
  CString(const char msg[], uint32_t len) : len_(len), p(new char[len]) {
    memcpy(p, msg, len);
  }
  ~CString() { delete[] p; }
  CString(const CString& orig) : len_(orig.len_), p(new char[len_]) {
    memcpy(p, orig.p, len_);
  }
  CString& operator=(const CString& orig) {
    CString copy(orig);
    swap(p, orig.p);
  }
  CString(CString&& orig) : len_(orig.len_) { swap(p, orig.p); }

  char operator[](uint32_t i) const { return p[i]; }
  char& operator[](uint32_t i) { return p[i]; }

  uint32_t len() const { return len_; }
  char* str() { return p; }

  friend std::ostream& operator<<(std::ostream& s, const CString& str) {
    for (int i = 0; i < str.len_; i++) s << str.p[i];
    return s;
  }
};
~
