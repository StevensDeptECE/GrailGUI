#pragma once

#include <glm/glm.hpp>

#include "data/BlockMapLoader.hh"
#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"

class Style;
class MapView2D : public Shape {
 private:
  const Style* style;
  float centerX, centerY, shiftX, shiftY, scaleX, scaleY;
  glm::mat4 originalTransform;
  glm::mat4 transform;
  // pointer to map loader object has advantage of opaqueness in header file
  // and we can have a null map and draw nothing, and change maps
  BlockMapLoader* bml;
  uint32_t numIndicesToDraw;

 public:
  void setProjection() {
    transform = glm::ortho(centerX - scaleX, centerX + scaleX, centerY - scaleY,
                           centerY + scaleY);
    getWin()->setDirty();
  }
  void translate(float percentX, float percentY) {
    centerX += percentX * scaleX;
    centerY += percentY * scaleY;
  }
  void uniformZoom(float s) { scaleX *= s, scaleY *= s; }
  MapView2D(Canvas* parent, const Style* s, BlockMapLoader* bml = nullptr)
      : Shape(parent), style(s), bml(bml), transform(1.0f) {
    const BlockMapLoader::BoundRect& bounds = bml->getBlockMapHeader()->bounds;
    centerX = (bounds.xMin + bounds.xMax) * 0.5;
    centerY = (bounds.yMin + bounds.yMax) * 0.5;

    double ySize = parent->getHeight();
    double xSize = parent->getWidth();
    shiftX = -bounds.xMin * xSize / (bounds.xMax - bounds.xMin);
    shiftY = ySize + (bounds.yMin * ySize / (bounds.yMax - bounds.yMin));
    scaleX = xSize / (bounds.xMax - bounds.xMin);
    scaleY = -ySize / (bounds.yMax - bounds.yMin);

    std::cout << "shift: " << shiftX << " " << shiftY << "\n";
    std::cout << "scale: " << scaleX << " " << scaleY << "\n";

#if 0
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(shiftX, shiftY, 0));
    transform = glm::scale(transform, glm::vec3(scaleX, scaleY, 0));
#endif
    this->centerX = -74, this->centerY = 40;
    this->scaleX = 70 / 2, this->scaleY = 70 / 2;
    setProjection();
  }
  glm::mat4& getTransform() { return transform; }
  void init() override;
  void render() override;
  void update() override;
  void dump();
};
