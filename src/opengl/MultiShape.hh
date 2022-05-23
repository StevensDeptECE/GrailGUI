#pragma once

#include <vector>

#include "opengl/Shape_impl.hh"

class MultiShape : public Shape {
 protected:
  std::vector<float> vertices;
  std::vector<uint32_t> solidIndices;
  std::vector<uint32_t> lineIndices;
  std::vector<uint32_t> pointIndices;
  std::vector<float> colorIndices;

 public:
  MultiShape(Canvas* parent, uint32_t vertCount = 1024,
             uint32_t solidIndCount = 1024, uint32_t lineIndCount = 1024,
             uint32_t pointIndCount = 1024, uint32_t colorIndCount = 1024)
      : Shape(parent) {
    vertices.reserve(vertCount * 2);
    solidIndices.reserve(solidIndCount);
    lineIndices.reserve(lineIndCount);
    pointIndices.reserve(pointIndCount);
    colorIndices.reserve(colorIndCount);
  }
  ~MultiShape();
  void process_input(Inputs* in, float dt) override;
  void update() override;
  void updatePoints();
  void updateSolidIndices();
  void updateLineIndices();
  void updatePointIndices();

  void addPoint(float x, float y) {
    vertices.push_back(x);
    vertices.push_back(y);
  }
};
