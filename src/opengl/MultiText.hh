#pragma once
#include <type_traits>

#include "opengl/GLWin.hh"
#include "opengl/GLWinFonts.hh"
#include "opengl/Shape.hh"
class Style;

class MultiText : public Shape {
 private:
  glm::mat4 transform;

 protected:
  uint32_t textureId;
  const Style* style;
  std::vector<float> vert;
  void addPoint(float x, float y, float u, float v) {
    vert.push_back(x);
    vert.push_back(y);
    vert.push_back(u);
    vert.push_back(v);
  }
  float velX = 1, velY = 1;
  float internalAdd(float x, float y, const Font* f, const char s[],
                    uint32_t len);
  uint32_t formatLead0(char destBuf[], uint8_t printVal) {
    uint32_t d0 = printVal / 100;
    destBuf[0] = '0' + d0;
    uint32_t rest = printVal - d0 * 100;
    uint32_t d1 = rest / 10;
    destBuf[1] = '0' + d1;
    uint32_t d2 = rest - d1 * 10;
    destBuf[2] = '0' + d2;
    return 3;
  }
  uint32_t formatLead0(char destBuf[], uint16_t printVal) {
    if (printVal >= 100) {
      uint32_t upper = printVal / 100;
      uint32_t d0 = upper / 100;
      destBuf[0] = '0' + d0;
      uint32_t rest = upper - d0 * 100;
      uint32_t d1 = rest / 10;
      destBuf[1] = '0' + d1;
      uint32_t d2 = rest - d1 * 10;
      destBuf[2] = '0' + d2;
    } else {
      destBuf[0] = '0';
      destBuf[1] = '0';
      destBuf[2] = '0';
    }
    printVal %= 100;
    uint32_t d0 = printVal / 10;
    destBuf[3] = '0' + d0;
    destBuf[4] = '0' + (printVal - d0 * 10);
    return 5;
  }
  uint32_t numDigits(uint64_t) { return 18; }
  uint32_t numDigits(uint32_t) { return 10; }
  uint32_t numDigits(uint16_t) { return 5; }
  uint32_t numDigits(uint8_t) { return 3; }
  uint32_t numDigits(unsigned long long x) { return numDigits(uint64_t(x)); }
  uint32_t numDigits(int) { return 10; }
  uint32_t numDigits(float) { return 10; }
  uint32_t numDigits(double) { return 21; }
  uint64_t biggestpow10(uint64_t) { return 1000000000000000000ULL; }
  uint32_t biggestpow10(uint32_t) { return 1000000000U; }
  uint32_t biggestpow10(uint16_t) { return 10000U; }
  uint32_t biggestpow10(uint8_t) { return 10000U; }
  uint64_t biggestpow10(unsigned long long x) {
    return biggestpow10(uint64_t(x));
  }
  uint64_t unsignit(unsigned long long int v) { return uint64_t(v); }
  uint64_t unsignit(int64_t v) { return uint64_t(v); }
  uint32_t unsignit(int32_t v) { return uint32_t(v); }
  uint16_t unsignit(int16_t v) { return uint16_t(v); }
  uint8_t unsignit(int8_t v) { return uint8_t(v); }
  uint64_t unsignit(uint64_t v) { return v; }
  uint32_t unsignit(uint32_t v) { return v; }
  uint16_t unsignit(uint16_t v) { return v; }
  uint8_t unsignit(uint8_t v) { return v; }

  template <typename T>
  uint32_t format(char destBuf[32], T printVal) {
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

  uint32_t format(char destBuf[32], float printVal, uint32_t precision = 8,
                  uint32_t afterdec = 2) {
#if 0
  //TODO: efficient right-to-left display of decimal using integer conversion
    int32_t decimalPos;
    frexp(printVal, &decimalPos);
    float pow10 = decimalPos * 0.30102999566; //convert to base 10
    uint32_t intVal = uint32_t(printVal * pow(10, pow10));
    return format(destBuf, intVal);
#endif

    uint32_t len = sprintf(destBuf, "%f", printVal);
    memcpy(destBuf + 32 - len, destBuf, len - 1);
    return len;
  }

  uint32_t format(char destBuf[32], double printVal) {
    uint32_t len = sprintf(destBuf, "%.15lf", printVal);
    memcpy(destBuf + 32 - len, destBuf, len - 1);
    return len;
  }

  uint32_t format(char destBuf[32], int printVal) {
    if (printVal < 0) {
      // TODO: this will not work for -intmax?!?
      uint32_t len = format(destBuf, uint32_t(-printVal));
      destBuf[32 - len] = '-';
      return len + 1;
    } else {
      return format(destBuf, uint32_t(printVal));
    }
  }

 public:
  MultiText(Canvas* c, const Style* style);
  MultiText(Canvas* c, const Style* style, uint32_t size);
  MultiText(Canvas* c, const Style* style, float angle, float x, float y);
  MultiText(Canvas* c, const Style* style, uint32_t size, float angle, float x,
            float y);
  ~MultiText();
  template <typename T>
  float addx(float x, float y, const Font* f, T printVal) {
    char buf[32];
    uint32_t len = format(buf, printVal);
    return internalAdd(x, y, f, buf + (sizeof(buf) - len), len);
  }

  template <typename T>
  float addCentered(float x, float y, float w, const Font* f, T printVal) {
    char buf[32];
    uint32_t len = format(buf, printVal);
    float textWidth = f->getWidth(buf + (sizeof(buf) - len), len);
    return internalAdd(x + (w - textWidth) / 2, y, f, buf + sizeof(buf) - len,
                       len);
  }

  template <typename T>
  float addRight(float x, float y, float w, const Font* f, T printVal) {
    char buf[32];
    uint32_t len = format(buf, printVal);
    float textWidth = f->getWidth(buf + (sizeof(buf) - len), len);
    internalAdd(x + w - textWidth, y, f, buf + sizeof(buf) - len, len);
    return x + w;
  }

  template <typename T>
  void addHex0(float x, float y, const Font* f, T printVal) {
    constexpr uint32_t len = sizeof(T) * 2;
    char buf[len];
    auto v = unsignit(printVal);  // use unsigned value so sign doesn't mess up
                                  // as we rotate bits
    for (uint32_t i = 0; i < len; i++) {
      v = (v << 4) | (v >> (len * 4 - 4));  // rotate 4 bits left
      uint32_t hexDig = v & 0xF;
      buf[i] = (hexDig > 9 ? 'A' - 10 : '0') + hexDig;
    }
    internalAdd(x, y, f, buf, len);
  }
  void addChar(float x, float y, const Font* f, const unsigned char c);
  // a 16-bit unicode character like Java
  void addChar(float x, float y, const Font* f, const uint16_t c);
  float add(float x, float y, float space, float ang, const Font* f,
           const char s[], uint32_t len);
  float add(float x, float y, const char s[], uint32_t len);
  float add(float x, float y, const Font* f, const char s[], uint32_t len);
  float add(float x, float y, uint32_t v);
  float add(float x, float y, uint64_t v);
  float add(float x, float y, const Font* f, uint32_t v);
  float addHex(float x, float y, const Font* f, uint32_t v);
  float addHex8(float x, float y, const Font* f, uint32_t v);
  float add(float x, float y, const Font* f, int32_t v);
  float add(float x, float y, float v);
  float add(float x, float y, const Font* f, float v);
  float add(float x, float y, double v);
  float add(float x, float y, const Font* f, double v);
  float add(float x, float y, const Font* f, double v, int fieldWidth,
           int precision);
  float add(float x, float y, const Font* f, const std::string& s);
  void addCentered(float x, float, const Font* f, double v, int fieldWidth,
                   int precision);
  void addCentered(float x, float y, float w, const Font* f, const char s[],
                   uint32_t len);
  void addCentered(float x, float y, float w, const Font* f,
                   const std::string& s);
  void addCentered(float x, float y, float w, float h, const Font* f,
                   const char s[], uint32_t len);
  void addCentered(float x, float y, const Font* f, const char s[],
                   uint32_t len);

  void checkAdd(float& x, float& y, const Font* f, const unsigned char c,
                float endMargin, float rowSize, float startOverMargin);
  uint32_t findFirstOverMargin(float x, const Font* f, const char s[],
                               uint32_t len, float rightMargin);
  void clear() { vert.clear(); }
  void init() override;

  void process_input(Inputs* in, float dt) {
#if 0
    if(x < 0 || x > GLWin::Width){
       velX =- velX;
      }   
      if(x < 0 || x > GLWin::Height){
        velY *= -1;  //TODO: WTF? Why would you change Y direction when you head to end of line? Seems like a bug
      }
    // x+=velX;
    // y+=velY;
#endif
  }
  const Style* getStyle();
  void update() override;
  void render() override;
};
