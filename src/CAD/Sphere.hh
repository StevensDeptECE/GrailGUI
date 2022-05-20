#pragma once
#include "opengl/Shape.hh"
//#include "opengl/Style.hh"

class Style;

class Sphere : public Shape {
 private:
  const Style* style;
  std::vector<float> vert;
  std::vector<uint32_t> ind;
  uint32_t latRes, longRes, radius;
  void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);
  void addTri(uint32_t i1, uint32_t i2, uint32_t i3);

 public:
  Sphere(Canvas* c, const Style* s, uint32_t latRes, uint32_t longRes,
         uint32_t radius);
  ~Sphere();
  void init();
  void render();
  void cleanup();
  void update();
};