#pragma once

#include "opengl/Shape2D.hh"

class Circle2D : public Shape2D{
private:
  float x, y, radius, segments;
public:
  Circle2D(Canvas* c, float x,float y, float rad, float segments, const Style* s); 

  void init();
  void render();
  void drawCircle();
  //void findArea();
  //void changeRadius(float newRadius);

};
