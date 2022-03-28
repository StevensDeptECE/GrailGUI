#include "CAD/RectangularPrism.hh"

#include "opengl/GLMath.hh"

RectangularPrism::RectangularPrism(Canvas* parent, uint32_t length,
                                   uint32_t width, uint32_t height)
    : Shape(parent), length(length), width(width), height(height) {}
RectangularPrism::~RectangularPrism() {}

inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3,
                                      uint32_t i4) {
  ind.push_back(i1);
  ind.push_back(i2);
  ind.push_back(i3);
  ind.push_back(i1);
  ind.push_back(i3);
  ind.push_back(i4);
}

void RectangularPrism::init() {}

double RectangularPrism::getVolume() {
  double volume = length * width * height;
  return volume;
}

void RectangularPrism::render() {}

void RectangularPrism::cleanup() {}