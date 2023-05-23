uint32_t dig4ToASCII[10000];
uint32_t leadingSpacedig4ToASCII[10000];

// 1234567 // leading digits are one big problem
void init() {
  for (int i = 0; i < 10000; i++) {
    dig4ToASCII[i] = (i / 1000 + '0') | ((i / 100 % 10 + '0') << 8) |
                     ((i / 10 % 10 + '0') << 16) | (i % 10 + '0') << 24;
  }
}

uint32_t getLength(uint32_t v) {
  int counter = 0;
  while (v) {
    v = v / 10;
    counter++;
  }
  return (counter);
}

void fastFormat(uint32_t v, char* p) {  //[0-42 9496 7295]
}

// 12345  --> p[0] = '1'  p[1] = '2'
void fastFormat(uint64_t v, char* p) {  //[0-1844 6744 0737 0955 1615]
  if (v >= 10000) {
    if (v >= 10000000) {
      // dd 10 000 000
      // 00 1000 00001
      top = v / 10000000;
      remain = v - top * 10000000;
      first4 = remain / 10000;
      last4 = remain - first4 * 10000;
      *(uint32_t*)p = dig4ToASCII[first4];
      p += 4;
      *(uint32_t*)p = dig4ToASCII[last4];
      p += 4;
    } else {
      //
    }
  } else {
    // 9999 9998 1000 10 2
  }
}

// 12345.1241e+203
// 1.23456
// 1234.56
void fastFormat(double v, char* p) {
  if (v > 10000) {
    if (v > 10000000) {
    } else {
    }
  }
}

#if 0
template<typename T>
void fastFormat(WebCursor& c, uint32_t v, T& text) {
 text.write4(WebCursor& c, dig4ToASCII[i]);

}
#endif

in main() { init(); }
