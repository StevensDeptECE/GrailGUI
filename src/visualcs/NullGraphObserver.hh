#pragma once

class NullGraphObserver {
public:
  void addVert(uint32_t index, float x, float y, uint8_t initialVertColor, std::vector<Point>&) {}
  void addEdge(uint64_t i, float x1, float y1, float x2, float y2, uint32_t initialEdgeColor) {}
  void updateVert(uint32_t i, uint8_t colorVal) {}
  void updateEdge(uint64_t j, uint8_t colorVal) {}
};
