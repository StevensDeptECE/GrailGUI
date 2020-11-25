#pragma once
#include "opengl/GLWin.hh"
#include "opengl/Shape.hh"
#include "opengl/GLWinFonts.hh"
class Style;

class PageText : public Shape {
private:
  uint32_t textureId;
  const Style* style;
  std::string text;
  std::vector<float> vert;
  void addPoint(float x, float y, float u, float v){
    vert.push_back(x);  vert.push_back(y);
    vert.push_back(u);  vert.push_back(v);
  }
  float velX = 1,velY=1;
public:
  MultiText(const Style * style, uint32_t size);
  void clear() { vert.clear(); }
  void loadText(const char s[], uint32_t len);
  void addChar(float x, float y, Font* f, const char c);
  void add(float x, float y, const char s[], uint32_t len);
  void add(float x, float y, uint32_t v);
  void add(float x, float y, float v);
  void add(float x, float y, double v);
  void init();
  void process_input(Inputs* in, float dt);
  void update();
  void render();
};
