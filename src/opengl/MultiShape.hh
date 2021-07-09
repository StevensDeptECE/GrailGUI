#pragma once

#include <vector>

#include "opengl/Shape_impl.cc"

class MultiShape : public Shape {
 protected:
  std::vector<float> vertices;
  std::vector<uint32_t> solidIndices;
  std::vector<uint32_t> lineIndices;
  std::vector<uint32_t> pointIndices;
  std::vector<float> colorIndices;
  uint32_t lineWidth;

  uint32_t getPointIndex(const uint32_t elemPerVert) const {
    return vertices.size() / elemPerVert;
  }

  // Utility
  void sAddTriIndices(const uint32_t elemPerVert);
  void sAddQuadIndices(const uint32_t elemPerVert);
  void sAddSectorIndices(uint32_t centerIndex, uint32_t indexCount,
                         const uint32_t elemPerVert);

  void lAddTriIndices(const uint32_t elemPerVert);
  void lAddQuadIndices(const uint32_t elemPerVert);
  void lAddSectorIndices(uint32_t centerIndex, uint32_t indexCount,
                         const uint32_t elemPerVert);

  void pAddTriIndices(const uint32_t elemPerVert);
  void pAddQuadIndices(const uint32_t elemPerVert);
  void pAddSectorIndices(uint32_t centerIndex, uint32_t indexCount,
                         const uint32_t elemPerVert);

 public:
  MultiShape(Canvas* parent, uint32_t lineWidth = 1, uint32_t vertCount = 1024,
             uint32_t solidIndCount = 1024, uint32_t lineIndCount = 1024,
             uint32_t pointIndCount = 1024, uint32_t colorIndCount = 1024)
      : Shape(parent), lineWidth(lineWidth) {
    vertices.reserve(vertCount * 2);
    solidIndices.reserve(solidIndCount);
    lineIndices.reserve(lineIndCount);
    pointIndices.reserve(pointIndCount);
    colorIndices.reserve(colorIndCount);
  }
  ~MultiShape();
  void process_input(Inputs* in, float dt) override;
  void update() override;
  void addPoint(float x, float y) {
    vertices.push_back(x);
    vertices.push_back(y);
  }
};
