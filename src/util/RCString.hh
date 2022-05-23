#pragma once
#include <memory.h>

#include <algorithm>
#include <iostream>
/*
        relative pointer, constant-size string, light weight
        Allocate a single static 4Gb chunk of memory in address space
        and allocate out of that
        Individual memory is not collected, the policy is free to mark
        This is ideal for situations where a lot of strings are allocated, and
   then deallocated all at the same time.

        This code uses a global pool, so allocation and deallocation is
        NOT thread safe. As a result, it is BRUTALLY FAST.
*/
class RCString {
 private:
  static char* pool;
  static uint32_t current;
  uint32_t len_;  // length of this string
  uint32_t p;     // relative pointer into pool
 public:
  // remember where allocator is up to right now
  static uint32_t setMark() { return current; }

  // free all memory going back to mark
  static void freeToMark(uint32_t mark) { current = mark; }

  RCString(const char msg[], uint32_t len) : len_(len), p(current) {
    current += len;
    memcpy(pool + p, msg, len);
  }
  RCString(const char msg[]) : p(current) {
    int i;
    for (i = 0; msg[i] != '\0'; i++) pool[current++] = msg[i];
    len_ = i;
  }
  ~RCString() {}
  RCString(const RCString& a, const RCString& b, const RCString& c)
      : len_(a.len_ + b.len_ + c.len_), p(current) {
    memcpy(pool + current, pool + a.p, a.len_);
    current += a.len_;
    memcpy(pool + current, pool + b.p, b.len_);
    current += b.len_;
    memcpy(pool + current, pool + c.p, c.len_);
    current += c.len_;
  }
  RCString(const RCString& orig) : len_(orig.len_), p(current) {
    current += len_;
    memcpy(pool + p, pool + orig.p, len_);
  }
  RCString& operator=(const RCString& orig) {
    RCString copy(orig);
    std::swap(p, copy.p);
    return *this;
  }
  RCString(RCString&& orig) : len_(orig.len_) { std::swap(p, orig.p); }

  char operator[](uint32_t i) const { return pool[p + i]; }
  char& operator[](uint32_t i) { return pool[p + i]; }

  uint32_t len() const { return len_; }

  friend std::ostream& operator<<(std::ostream& s, const RCString& str) {
    for (int i = 0; i < str.len_; i++) s << RCString::pool[str.p + i];
    return s;
  }
};
