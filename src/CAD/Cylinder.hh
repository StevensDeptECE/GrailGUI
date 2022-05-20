#pragma once

#include "opengl/Shape.hh"
//#include "opengl/Canvas.hh"

class Style;

class Cylinder : public Shape {
 private:
  std::vector<float> vert;
  std::vector<float> unitVert;
  std::vector<uint32_t> ind;
  void getUnitCircleVertices();
  uint32_t height, radius, x, y, z, segments;
  const Style* style;

 public:
  // Cylinder(uint32_t height, uint32_t r1, uint32_t r2);
  Cylinder(Canvas* c, const Style* s, uint32_t height, uint32_t x, uint32_t y,
           uint32_t z, uint32_t radius, uint32_t segments);
  ~Cylinder();
  double getVolume();
  void init();
  void render();
  void cleanup();
  void update();
};