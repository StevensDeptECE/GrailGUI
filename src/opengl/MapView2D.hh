#pragma once

#include <glm/glm.hpp>

#include "opengl/BlockMapLoader.hh"
#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"

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
      : Shape(parent), style(s), bml(bml), transform(1.0f) {
    const BlockMapLoader::BoundRect& bounds = bml->getBlockMapHeader()->bounds;
    float centerX = (bounds.xMin + bounds.xMax) * 0.5;
    float centerY = (bounds.yMin + bounds.yMax) * 0.5;

    double ySize = parent->getHeight();
    double xSize = parent->getWidth();
    double shiftX = -bounds.xMin * xSize / (bounds.xMax - bounds.xMin);
    double shiftY = ySize + (bounds.yMin * ySize / (bounds.yMax - bounds.yMin));
    double scaleX = xSize / (bounds.xMax - bounds.xMin);
    double scaleY = -ySize / (bounds.yMax - bounds.yMin);

    std::cout << "shift: " << shiftX << " " << shiftY << "\n";
    std::cout << "scale: " << scaleX << " " << scaleY << "\n";

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(shiftX, shiftY, 0));
    transform = glm::scale(transform, glm::vec3(scaleX, scaleY, 0));
  }
  void init() override;
  void render() override;
  void update() override;
  void dump();
};
