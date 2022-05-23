#pragma once
#include "opengl/Shape.hh"
#incldue "opengl/Polygon.hh"

class Prism : public Shape {
 private:
  std::vector<float> vert;
  std::vector<uint32_t> ind;

 public:
  Prism(Polygon polygon, uint32_t height);
  ~Prism();
  double getVolume();  // Base area * height
  void init();
  void render();
  void cleanup();
};