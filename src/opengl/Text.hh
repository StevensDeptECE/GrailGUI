#pragma once
#include "opengl/GLWin.hh"
#include "opengl/primitivesV2/Shape.hh"
#include "opengl/GLWinFonts.hh"
class Style;

class Text : public Shape {
private:
  const Font* f;
  uint32_t textureId;
  float x,y;
  Style* style;
  std::string text;
  std::vector<float> vert;
  void addPoint(float x, float y, float u, float v){
    vert.push_back(x);  vert.push_back(y);
    vert.push_back(u);  vert.push_back(v);
  }
  float velX = 1,velY=1;
public:
  Text(float x, float y, Style * style, const std::string& text) : Shape(),x(x),y(y),style(style),text(text){}
  ~Text();
  void init();
  void update();
  void render();
  const static uint32_t LINE_LENGTH = 256;
};
