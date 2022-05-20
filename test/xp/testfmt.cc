#include <chrono>
#include <functional>
#include <iostream>

#include "digits.h"
#include "fmt/color.h"
#include "fmt/compile.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "util/Benchmark.hh"

using namespace std;
using namespace fmt;
using namespace grail::utils;

template <typename C, typename T>
constexpr uint32_t test_format(C destBuf, T fmt_val) {
  format_to(back_inserter(destBuf), FMT_COMPILE("{}"), fmt_val);
  return destBuf.size();  // size of the formatted data
}

uint32_t formatspf(char data[33], float f) { return sprintf(data, "%f", f); }

int32_t formatspf(char data[33], double f) { return sprintf(data, "%lf", f); }

uint32_t formatspf(char data[33], int f) { return sprintf(data, "%d", f); }

uint32_t formatspf(char data[33], uint32_t f) { return sprintf(data, "%u", f); }

uint32_t formatspf(char data[33], uint64_t f) {
  return sprintf(data, "%lu", f);
}

uint32_t formatspf(char data[33], const char* f) {
  return sprintf(data, "%s", f);
}

template <typename T>
void test_sprintf(char dest[33], T data) {
  formatspf(dest, data);
}

// R to L, 1 at a time
template <typename T>
constexpr uint32_t _formatgrl(char destBuf[33], T printVal) {
  uint32_t len;
  int i;
  for (i = 31; i >= 0; i--) {
    T temp = printVal / 10;
    uint32_t digit = printVal - temp * 10;
    destBuf[i] = '0' + digit;
    if (printVal == 0) break;
    printVal = temp;
  }
  return 31 - i;
}

// R to L, 2 at a time
template <typename T>
constexpr uint32_t _formatgrl2(char destBuf[33], T printVal) {
  uint32_t len;
  int i;
  for (i = 30; printVal >= 100; i -= 2) {
    T temp = printVal / 100;
    uint32_t digit2 = printVal - temp * 100;
    *(uint16_t*)&destBuf[i] = ((uint16_t*)dig2)[digit2];
    printVal = temp;
  }
  if (printVal >= 10) {
    *(uint16_t*)&destBuf[i] = ((uint16_t*)dig2)[printVal];
  } else if (printVal > 0) {
    destBuf[++i] = '0' + printVal;
  }
  return 32 - i;
}

// R to L, 4 at a time, in pairs
template <typename T>
constexpr uint32_t _formatgrl3(char destBuf[33], T printVal) {
  uint32_t len;
  int i;
  for (i = 28; printVal >= 10000; i -= 4) {
    T temp = printVal / 10000;
    uint32_t digit4 = printVal - temp * 10000;
    uint32_t digit2 = digit4 / 100;
    *(uint16_t*)&destBuf[i + 2] = ((uint16_t*)dig2)[digit4 - digit2 * 100];
    *(uint16_t*)&destBuf[i] = ((uint16_t*)dig2)[digit2];
    printVal = temp;
  }
  if (printVal >= 100) {
    if (printVal >= 1000) {
      uint32_t digit2 = printVal / 100;
      *(uint16_t*)&destBuf[i] = ((uint16_t*)dig2)[digit2];
      *(uint16_t*)&destBuf[i + 2] = ((uint16_t*)dig2)[printVal - digit2 * 100];
    } else {
      uint32_t digit = printVal / 100;
      *(uint16_t*)&destBuf[i + 2] = ((uint16_t*)dig2)[printVal - digit * 100];
      destBuf[i += 1] = '0' + digit;
    }
  } else {
    if (printVal >= 10) {
      *(uint16_t*)&destBuf[i += 2] = ((uint16_t*)dig2)[printVal];
    } else if (printVal > 0) {
      destBuf[i += 3] = '0' + printVal;
    }
  }
  return 32 - i;
}

// R to L, 4 at a time
template <typename T>
constexpr uint32_t _formatgrl4(char destBuf[33], T printVal) {
  uint32_t len;
  int i;
  for (i = 28; printVal >= 10000; i -= 4) {
    T temp = printVal / 10000;
    uint32_t digit4 = printVal - temp * 10000;
    *(uint32_t*)&destBuf[i] = ((uint32_t*)dig4)[digit4];
    printVal = temp;
  }
  if (printVal >= 100) {
    if (printVal >= 1000) {
      *(uint32_t*)&destBuf[i] = ((uint32_t*)dig4)[printVal];
    } else {
      *(uint16_t*)&destBuf[i + 2] = ((uint32_t*)dig4)[printVal];
      destBuf[i += 1] = (dig4)[(printVal << 2) + 2];
    }
  } else {
    if (printVal >= 10) {
      *(uint16_t*)&destBuf[i += 2] = ((uint16_t*)dig2)[printVal];
    } else if (printVal > 0) {
      destBuf[i += 3] = '0' + printVal;
    }
  }
  return 32 - i;
}

constexpr uint32_t _formatgrl4signed(char destBuf[33], int32_t v) {
  if (v < 0) {
    if (v == INT32_MIN) {
      //  24 ... 28 29 30 31        "-21 47483648"
      //  16 17 18 .. 21 22  23
      constexpr uint64_t INT32_MIN_LAST8DIGITS =
          '8' | ('4' << 8) | ('6' << 16) | ('3' << 24) | (uint64_t('8') << 32) |
          (uint64_t('4') << 40) | (uint64_t('7') << 48) | (uint64_t('4') << 56);
      constexpr uint32_t INT32_MIN_FIRST_DIGITS =
          ('1' | ('2' << 8) | ('-' << 16)) << 8;
      *(uint64_t*)(destBuf + 24) = INT32_MIN_LAST8DIGITS;
      *(uint32_t*)(destBuf + 20) = INT32_MIN_FIRST_DIGITS;
      return 21;
    }
    uint32_t size = _formatgrl4(destBuf, (uint32_t)-v);
    destBuf[size - 1] = '-';
    return size - 1;
  }
  return _formatgrl4(destBuf, v);
}

constexpr uint32_t _formatgrl4signed(char destBuf[33], int64_t v) {
  if (v < 0) {
    if (v == INT64_MIN) {
      //  24 ... 28 29 30 31        "-922 33720368 54775808"
      //  16 17 18 .. 21 22  23
      constexpr uint64_t INT64_MIN_LAST8DIGITS =
          '8' | ('0' << 8) | ('8' << 16) | ('5' << 24) | (uint64_t('7') << 32) |
          (uint64_t('7') << 40) | (uint64_t('4') << 48) | (uint64_t('5') << 56);
      constexpr uint64_t INT64_MIN_MID8DIGITS =
          '8' | ('6' << 8) | ('3' << 16) | ('0' << 24) | (uint64_t('2') << 32) |
          (uint64_t('7') << 40) | (uint64_t('3') << 48) | (uint64_t('3') << 56);
      constexpr uint32_t INT64_MIN_FIRST_DIGITS =
          '2' | ('2' << 8) | ('9' << 16) | ('-' << 24);
      *(uint64_t*)(destBuf + 24) = INT64_MIN_LAST8DIGITS;
      *(uint64_t*)(destBuf + 16) = INT64_MIN_MID8DIGITS;
      *(uint32_t*)(destBuf + 12) = INT64_MIN_FIRST_DIGITS;
      return 12;
    }
    uint32_t size = _formatgrl4(destBuf, (uint32_t)-v);
    destBuf[size - 1] = '-';
    return size - 1;
  }
  return _formatgrl4(destBuf, v);
}

constexpr uint32_t _formatgrl(char destBuf[33], int printVal) {
  if (printVal < 0) {
    // TODO: this will not work for -intmax?!?
    uint32_t len = _formatgrl(destBuf, uint32_t(-printVal));
    destBuf[32 - len] = '-';
    return len + 1;
  } else {
    return _formatgrl(destBuf, uint32_t(printVal));
  }
}

constexpr inline char DECIMAL_POINT = '.';
constexpr void _insertdec(char destBuf[33], uint32_t& i, uint32_t decimalLoc,
                          uint32_t digit4) {
  // load the right 4 digits from the table 0000 to 9999
  uint32_t digits = ((uint32_t*)dig4)[digit4];
  if (decimalLoc >= 4) {
    *(uint32_t*)&destBuf[i] = digits;
  } else {
    switch (decimalLoc) {
      case 0:
        destBuf[i + 3] = DECIMAL_POINT;
        *(uint32_t*)&destBuf[--i] = digits;
        break;
      case 1:
        destBuf[i + 3] = digits;
        destBuf[i + 2] = DECIMAL_POINT;
        i -= 2;
        digits <<= 8;
        // now there are only 3 bytes in the word, low byte is 0
        *(uint32_t*)&destBuf[i] = digits;
        // just wrote 4 bytes, but there were only 3
        // still, it's faster because writing 16 bits then 8 bits as two
        // separate memory writes and 2 separate instructions to do it.
        i++;
        break;
      case 2:
        //                 i-1   i i+1 i+2 i+3
        //                  d    d   .   d  d
        *(uint16_t*)&destBuf[i + 2] = digits;
        destBuf[i + 1] = DECIMAL_POINT;
        i--;
        digits <<= 16;
        *(uint16_t*)&destBuf[i] = digits;
        break;
      case 3:
        //                  i-1  i  i+1 i+2 i+3
        //                    d  .    d   d  d
        // write 4 bytes, one will get thrown out
        // rotate left by 8 bits so the first 3 numbers are in low 3 bytes first
        // time
        // digits = (digits << 8) |
        //         (digits >> 24);  //  11111111222222223333333300000000
        *(uint32_t*)&destBuf[i] = digits;
        destBuf[i] = DECIMAL_POINT;  // overwrite high byte with decimal point
        i--;
        // write out remaining 1 byte in the word
        destBuf[i] = digits;
        break;
    }
  }
}

// R to L, 4 at a time
constexpr uint32_t _formatgrlfloat(char destBuf[33], uint64_t printVal,
                                   uint32_t precision, uint32_t decimalLoc) {
  uint32_t len;
  uint32_t i;
  for (i = 28; precision >= 4; i -= 4, precision -= 4, decimalLoc -= 4) {
    uint64_t temp = printVal / 10000;
    // get the 4 digits from memory
    uint32_t digit4 = printVal - temp * 10000;
    printVal = temp;
    _insertdec(destBuf, i, decimalLoc, digit4);
  }
  _insertdec(destBuf, i, decimalLoc, printVal);
  i +=
      (printVal >= 100 ? (printVal >= 1000 ? 0 : 1) : (printVal >= 10 ? 2 : 3));

  return i;
}

template <floating_point T>
constexpr uint32_t _formatgrl5(char destBuf[33], T data) {
  int ex;
  constexpr int precision = 6;
  // 1.234567
  // 123.4567
  // .1234567e+23
  // .9923456
  // .9999999

  // 1234567.
  // 1234.567
  // 12.34567
  frexp(data, &ex);
  constexpr double invLog2_10 = 1 / log2(10.0);
  double powOf10 = ceil(ex * invLog2_10);
  T scale = pow(10, powOf10 - precision);
  T scaled = data / scale;
  // cout << "Scaled: " << scaled << '\n';  // .123456789 or 123456789
  uint64_t printVal = uint64_t(scaled);
  // cout << "Integer value" << printVal << '\n';
  // 123.450
  //   0.345
  uint32_t decimalLoc = precision - uint32_t(powOf10);
  return _formatgrlfloat(destBuf, printVal, precision, decimalLoc);
}

template <typename T>
constexpr void testboth(T data) {
  print(fg(color::steel_blue) | emphasis::bold, "Currently processing: {}\n",
        data);
  char dest[33];
  vector<char> out(33);
  constexpr uint64_t n = 1000000;
  CBenchmark<>::benchmark("fmtlib format", n,
                          bind(test_format<vector<char>, T>, out, data));
  CBenchmark<>::benchmark("sprintf format", n,
                          bind(test_sprintf<T>, dest, data));
  CBenchmark<>::benchmark("Grail float", n, bind(_formatgrl5<T>, dest, data));
  print("{}\n", data);
}

template <typename T>
constexpr void testAll(T data) {
  print(fg(color::steel_blue) | emphasis::bold, "Currently processing: {}\n",
        data);
  char dest[33];
  vector<char> out(33);
  constexpr uint64_t n = 1000000;

  CBenchmark<>::benchmark("fmtlib format", n,
                          bind(test_format<vector<char>, T>, out, data));
  CBenchmark<>::benchmark("sprintf format", n,
                          bind(test_sprintf<T>, dest, data));
  CBenchmark<>::benchmark("grail format RtoL 1 char", n,
                          bind(_formatgrl<T>, dest, data));
  CBenchmark<>::benchmark("grail format RtoL 2 char", n,
                          bind(_formatgrl2<T>, dest, data));
  CBenchmark<>::benchmark("grail format RtoL 4 char pairs", n,
                          bind(_formatgrl3<T>, dest, data));
  CBenchmark<>::benchmark("grail format RtoL 4 char", n,
                          bind(_formatgrl4<T>, dest, data));
  // Benchmark::benchmark("grail format RtoL 4char, sys benchmark", n,
  //                     bind(_formatgrl4<T>, dest, data));
  print("{}\n", data);
}

int main() {
  testboth(1.0f);
  testboth(135567.362626);

  testAll(11);
  testAll((uint32_t)-134135);
  testAll(UINT64_MAX);

  char dest[33];
  // s eeeeeeee mmmmmmmmmmmmmmmmmmmmmmm
  //   10000000
  dest[32] = '\0';
  uint8_t size = _formatgrl5(dest, 12323452345.45);
  cout << dest + size << '\n';
  testboth(123.45);
  // testboth("hello test");
}
