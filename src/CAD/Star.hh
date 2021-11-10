#pragma once

#include "opengl/Shape2D.hh"

class Star : public Shape2D{
private:
  float x1, y1, numberOfPoints;
public:
  Star(Canvas* c, float x, float y, float radius, float numberOfPoints, const Style* s);
  //void initIndices();
  void init();
  void render();
};
