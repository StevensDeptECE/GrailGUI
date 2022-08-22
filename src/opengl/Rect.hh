#pragma once

#include "opengl/Shape.hh"
#include "opengl/Style.hh"

class Rect : public Shape {
 private:
  float x, y, width, height;
  float r,g,b;
 public:
  Rect(Canvas* c, float x, float y, float width, float height, const Style* s)
      : Shape(c), width(width), height(height), r(s->fg.r), g(s->fg.b), b(s->fg.b) {}
  void init() override;
  void update() override;
  void render(glm::mat4& trans) override;
};
