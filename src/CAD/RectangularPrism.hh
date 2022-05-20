#pragma once

#include "opengl/Shape.hh"

class Style;

class RectangularPrism : public Shape {
 private:
  const Style* style;
  float xsize, ysize, zsize, x, y, z;
  std::vector<uint32_t> ind;
  std::vector<float> vert;
  void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);
  void addTri(uint32_t i1, uint32_t i2, uint32_t i3);
  void addVert(float x, float y, float z);

 public:
  RectangularPrism(Canvas* c, const Style* s, float xsize, float ysize,
                   float zsize, float x, float y, float z);
  ~RectangularPrism();
  double getVolume();
  std::vector<float> getVerts() { return vert; }
  void init();
  void render();
  void cleanup();
  void update();
};