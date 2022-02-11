#pragma once

#include "opengl/Shape2D.hh"

class Gear : public Shape2D{
private:
  float x, y, innerRadius, outterRadius, numTeeth, toothDepth, width;
  uint32_t numOfverticies = 362;
  float circleVerts[724];
public:
  Gear(Canvas* c, float x, float y, float innerradius, float outterradius, float numTeeth, float toothDepth, float width, const Style* s);
  void init();
  void render();
};
