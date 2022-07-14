#pragma once

#include <glm/glm.hpp>

#include "opengl/Shape.hh"

class HeatMap : public Shape {
 private:
  std::vector<float> grid; // all x,y values stored here
  std::vector<float> values; // floating point values for each region here

 public:
  HeatMap(uint32_t w, uint32_t h) {
    grid.reserve((w + 1) * (h + 1) * 2);
  }

  void add(float x, float y) {
    grid.push_back(x);
    grid.push_back(y);
  }

  ~HeatMap() override;
  void init() override;
  void render() override;
  void update() override;
};
