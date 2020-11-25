#pragma once

#include <glm/glm.hpp>

#include "opengl/Shape.hh"

class HeatMap : public Shape {
 private:
  std::vector<float> grid;
  uint32_t currentIndex;

 public:
  HeatMap(uint32_t w, uint32_t h) : currentIndex(0) {
    grid.reserve((w+1)*(h+1)*2);
  }

  void add(float x1, float y1, float v1,
           float x2, float y2, float v2,
           float x3, float y3, float v3,
           float x4, float y4, float v4) {
    grid.push_back(x1);
    grid.push_back(y1);
    grid.push_back(v1);

    grid.push_back(x2);
    grid.push_back(y2);
    grid.push_back(v2);

    grid.push_back(x3);
    grid.push_back(y3);
    grid.push_back(v3);

    grid.push_back(x2);
    grid.push_back(y2);
    grid.push_back(v2);

    grid.push_back(x3);
    grid.push_back(y3);
    grid.push_back(v3);

    grid.push_back(x4);
    grid.push_back(y4);
    grid.push_back(v4);
  }

  ~HeatMap() override;
  void init() override;
  void render() override;
  void update() override;
};
