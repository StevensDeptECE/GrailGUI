#pragma once

#include "opengl/Shape2D.hh"

class Rectangle : public Shape2D{
private:
  float x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
public:
  Rectangle(Canvas* c, float x1, float y1, float width, float height, const Style* s);
  void init();
  void render();
  //float getArea();
};