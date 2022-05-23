#pragma once

#include "opengl/Shape2D.hh"

class Circle2D : public Shape2D {
 private:
  float x, y, radius;
  uint32_t numOfTriangles;
  uint32_t numOfverticies = numOfTriangles + 2;
  float circleVerts[724];

 public:
  Circle2D(Canvas* c, float x, float y, float rad, uint32_t numOfTriangles,
           const Style* s);

  void init();
  void render();
  void drawCircle();
};
