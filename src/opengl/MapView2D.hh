#pragma once

#include <glm/glm.hpp>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/BlockMapLoader.hh"

class Style;
class MapView2D : public Shape {
 private:
  const Style* style;
  glm::mat4 transform;
  // pointer to map loader object has advantage of opaqueness in header file
  // and we can have a null map and draw nothing, and change maps
  BlockMapLoader* bml;
  uint32_t numPoints;

 public:
  MapView2D(Canvas* parent, const Style* s, BlockMapLoader* bml = nullptr)
      : Shape(parent), style(s), bml(bml), transform(1.0) {
        const BlockMapLoader::BoundRect& bounds = bml->getBlockMapHeader()->bounds;
        float centerX = (bounds.xMin + bounds.xMax) * 0.5;
        float centerY = (bounds.yMin + bounds.yMax) * 0.5;
        transform = glm::translate(transform, glm::vec3(centerX, centerY,0));
        transform = glm::scale(transform, glm::vec3(2,2,1));
  }
  void init() override;
  void render() override;
  void update() override;
  void dump();
};
