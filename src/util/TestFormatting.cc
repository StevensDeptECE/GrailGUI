#include "util/DataType.hh"
#include "util/Format.hh"

class TestRecord {
 private:
  uint8_t a;
  uint16_t b;
  uint32_t c;
  uint64_t d;
  int8_t e;
  int16_t f;
  int32_t g;
  int64_t h;
  float i;
  double j;
  //	Date date;
  //  JulianDate jdate;
  char str1[4];
  char str2[25];

 public:
  TestRecord() {
    a = 123;
    b = 45678;
    c = 4123456789;
    d = 123456789012345678LL;
    e = -123;
    f = -25678;
    g = -2103456789;
    h = -123456789012345678LL;
    i = 1.234567;
    j = -12345.6789012345;
    str1[0] = 3;
    str1[1] = 'a';
    str1[2] = 'b';
    str1[3] = 'c';

    str2[0] = '\0x0A';
    str2[1] = '\0x00';  // 10 characters
    str2[2] = 'h';
    str2[3] = 'e';
    str2[4] = 'l';
    str2[5] = 'l';
    str2[6] = 'o';
    str2[7] = 'x';
    str2[8] = 'x';
    str2[9] = 'x';
    str2[10] = 'j';
    str2[11] = 'q';
  }
};

/*
        void format(float x, float y, Style* style) {
        uint8_t v = *(uint8_t*)((char*)p + var);


print out integers as dec, as hex



 */

int main() {
  TestRecord r;
  // printf("%6d",
  Format f1(&r, 0, DataType::U8, "%6d");
  f1.format(x, y, nullptr);  // ___123
  Format f2(&r, 0, DataType::U16, "%8d");
  f2.format(x, y, nullptr);  // ___45678

  Format f10(&r, 0, DataType::DOUBLE, "%16.l0f");
