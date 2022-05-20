#include <cassert>
#include <iostream>

#include "util/Buffer.hh"
/**
         @author Lin

 */

/*
 * Basic test of writing/reading data
 */

/*
describe

 */
void test1() {
  Buffer buf("test1.dat", 32768);
  uint8_t a = 1;
  uint16_t b = 2;
  uint32_t c = 257;  // 1 in first and second byte low-endian
  uint64_t d = 1024 + 1024 * 1024 + 1024 * 1024 * 1024;
  string e = "abcd";
  buf << a << b << c << d << e;  // 1 + 2 + 4 + 8 + 5 bytes for string
  cout << "test" << endl;
  buf.flush();
  // assert(buf.getBytesUsed() == 20);
}

/*
        describe test here please!

 */

void test2() {
  Buffer buf("test1.dat", 32768, "IN");
  uint8_t a = buf.read<uint8_t>();
  uint16_t b = buf.read<uint16_t>();
  uint32_t c = buf.read<uint32_t>();
  uint64_t d = buf.read<uint64_t>();
  string e = buf.readString8();
  assert(a == 1);
  assert(b == 2);
  assert(c == 257);
  assert(d == 1024 + 1024 * 1024 + 1024 * 1024 * 1024);
  assert(e == "abcd");
}

void test4() {
  Buffer buf("test2.dat", 32);
  uint8_t a = 1;
  uint16_t b = 2;
  uint32_t c = 257;  // 1 in first and second byte low-endian
  uint64_t d = 1024 + 1024 * 1024 + 1024 * 1024 * 1024;
  string e = "abcd";
  buf << a;
  for (uint32_t i = 0; i < 20; i++) buf << i;
  buf << b << c << d << e;  // 2 + 4 + 8 + 5 bytes for string
                            // assert(buf.getTotalBytesSent() == 100);
}
void test5() {
  Buffer buf("test2.dat", 32, "IN");
  uint8_t a = buf.read<uint8_t>();
  uint32_t v;
  for (int i = 0; i < 20; i++) v = buf.read<uint32_t>();
  uint16_t b = buf.read<uint16_t>();
  uint32_t c = buf.read<uint32_t>();
  uint64_t d = buf.read<uint64_t>();
  string e = buf.readString8();
  assert(a == 1);
  assert(b == 2);
  assert(c == 257);
  assert(d == 1024 + 1024 * 1024 + 1024 * 1024 * 1024);
  assert(e == "abcd");
}

#if 0
void test3() {
    Buffer buf("test1.dat", 32768, "IN");

    char str[256];
    char*s = str;
    buf.appendUint8(&s);
    buf.append
}
#endif

int main() {
  cout << "test1" << endl;
  test1();
  cout << "test2" << endl;
  test2();
  cout << "test4" << endl;
  test4();
  cout << "test5" << endl;
  test5();

  return 0;
}
