#pragma once
#include "opengl/Shape.hh"

class Sphere : public Shape {
 private:
  std::vector<float> vert;
  std::vector<uint32_t> ind;
  void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);
  void addTri(uint32_t i1, uint32_t i2, uint32_t i3);
  void addVert(float x, float y, float z);  // add just coords
  void addVert(float x, float y, float z, float u,
               float v);  // add with texture
 public:
  Sphere(uint32_t latRes, uint32_t lonRes);
  ~Sphere();
  void init();
  void render();
  void cleanup();
};
