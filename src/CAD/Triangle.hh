#pragma once

#include "opengl/Shape2D.hh"

class Triangle : public Shape2D{
private:
  float x1, y1, x2, y2, x3, y3;
public:
  Triangle(Canvas* c, float x, float y, float base, float height, const Style* s);
  Triangle(Canvas* c, float x1, float y1, float x2, float y2,float x3, float y3, const Style* s);
  //void initIndices();
  void init();
  void render();
  //void getArea(base, height);
  //void getArea(x, y, x2, y2, x3, y3);
};

