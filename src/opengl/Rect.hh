#pragma once

#include "opengl/Shape.hh"
#include "opengl/Style.hh"

class Rect : public Shape {
 private:
  float x, y, width, height;
  float r, g, b;

 public:
  Rect(Canvas* c, float x, float y, float width, float height, float r, float g,
       float b)
      : Shape(c), x(x), y(y), width(width), height(height), r(r), g(g), b(b) {}
  void init() override;
  void update() override;
  void render(glm::mat4& trans) override;
};
