#pragma once
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

 public:
  MultiText(Canvas* c, const Style* style);
  MultiText(Canvas* c, const Style* style, uint32_t size);
  MultiText(Canvas* c, const Style* style, float angle, float x, float y);
  MultiText(Canvas* c, const Style* style, uint32_t size, float angle, float x,
            float y);
  ~MultiText();
  void addChar(float x, float y, const Font* f, const unsigned char c);
  // a 16-bit unicode character like Java
  void addChar(float x, float y, const Font* f, const uint16_t c);
  void add(float x, float y, const char s[], uint32_t len);
  void add(float x, float y, const Font* f, const char s[], uint32_t len);
  void add(float x, float y, uint32_t v);
  void add(float x, float y, const Font* f, uint32_t v);
  void addHex(float x, float y, const Font* f, uint32_t v);
  void addHex8(float x, float y, const Font* f, uint32_t v);
  void add(float x, float y, const Font* f, int32_t v);
  void add(float x, float y, float v);
  void add(float x, float y, const Font* f, float v);
  void add(float x, float y, double v);
  void add(float x, float y, const Font* f, double v);
  void add(float x, float y, const Font* f, double v, int fieldWidth,
           int precision);
  void addCentered(float x, float, const Font* f, double v, int fieldWidth,
                   int precision);
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
