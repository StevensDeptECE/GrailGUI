#pragma once

#include "opengl/Shape2D.hh"

class Circle2D : public Shape2D{
private:
  float x, y, radius;
  uint32_t segments;
  float allVerts[362*2];
public:
  Circle2D(Canvas* c, float x,float y, float rad, uint32_t segments, const Style* s); 

  void init();
  void render();
  void drawCircle();

};
