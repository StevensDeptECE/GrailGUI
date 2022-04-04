#pragma once
#include <vector>

#include "opengl/Widget2D.hh"

class PositionTool : public Widget2D {
 private:
  std::vector<float> xCord;
  std::vector<float> yCord;
  std::vector<float> radius;

 public:
  PositionTool(StyledMultiShape2D* m, MultiText* t, float x, float y, float w,
               float h, std::vector<float>& X, std::vector<float>& Y,
               std::vector<float>& r)
      : Widget2D(m, t, x, y, w, h), xCord(X), yCord(Y), radius(r) {}

  void addNewCord(float newX, float newY, float newR);

  void update(float newX, float newY, uint32_t position);

  void init() override;
};
