#pragma once

#include <glm/glm.hpp>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"

class BlockMapLoader;
class MapView2D : public Shape {
 private:
  glm::mat4 transform;
  // pointer to map loader object has advantage of opaqueness in header file
  // and we can have a null map and draw nothing, and change maps
  BlockMapLoader* bml;
  uint32_t numPoints;
  uint32_t vbox, vboy;

 public:
  MapView2D(Canvas* parent, const Style* s, BlockMapLoader* bml = nullptr)
      : Shape(parent), bml(bml), transform(1.0) {}
  void init() override;
  void render() override;
  void dump();
};
