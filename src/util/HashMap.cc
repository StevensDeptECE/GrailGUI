#include "util/HashMap.hh"

uint32_t HashMapBase::bytewisehash(const char s[]) const {
  uint32_t i;
  uint32_t sum = s[0] ^ 0x56F392AC;
  for (i = 1; s[i] != '\0'; i++) {
    sum = (((sum << r1) | (sum >> (32-r1))) ^ ((sum >> r2) | (sum >> (32-r2)))) + s[i];
    sum = (((sum << r3) | (sum >> (32-r3))) ^ ((sum >> r4) | (sum >> (32-r4)))) + s[i];
  }
  sum = ((sum << r5) | (sum >> (32-r5))) ^ sum ^ (i << 7);
//  sum = (((sum << r5) | (sum >> (32-r5))) ^ ((sum << r6) | (sum >> (32-r6)))) + s[i];
  return sum & size;
}

// do not call with len = 0, or die.
uint32_t HashMapBase::bytewisehash(const char s[], uint32_t len) const {
  uint32_t i;
  uint32_t sum = s[0] ^ 0x56F392AC;
  for (i = 1; i < len; i++) {
    sum = (((sum << r1) | (sum >> (32-r1))) ^ ((sum >> r2) | (sum >> (32-r2)))) + s[i];
    sum = (((sum << r3) | (sum >> (32-r3))) ^ ((sum >> r4) | (sum >> (32-r4)))) + s[i];
  }
  sum = ((sum << r5) | (sum >> (32-r5))) ^ sum ^ (i << 7);
//  sum = (((sum << r5) | (sum >> (32-r5))) ^ ((sum << r6) | (sum >> (32-r6)))) + s[i];
  return sum & size;
}

uint32_t HashMapBase::fasthash1(const char s[]) const {
  uint64_t* p = (uint64_t*)s;
  uint64_t v;
  uint64_t sum = 0xF39A5EB6;
  
  //https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
  while (v = *p++, hasNoZero(v)) {
    // rotate might be better, preserve more bits in each operation
    sum = sum ^ v ^ (v << 12);
    sum = (sum << 3) ^ (sum >> 5);
    sum = (sum << 7) ^ (sum >> 13);
    sum = (sum >> 17) ^ (sum << 23);
  }
  // do the last chunk which is somewhere less than 8 bytes
  // this is for little-endian CPUs like intel, from bottom byte to the right
  uint64_t wipe = 0xFF;
  uint64_t M = 0xFFULL;
  for (int i = 8; i > 0; i--) {
    if ((v & M) == 0) {
      v &= wipe;
      break;
    }
    M <<= 8;
    wipe = (wipe << 8) | 0xFF;
  }
  sum = sum ^ v ^ (sum >> 3);
  sum = (sum >> 7) ^ (sum << 9);
  sum = (sum >> 13) ^ (sum << 17);
  sum = (sum >> 31) ^ (sum >> 45);
  return sum & size;
}
