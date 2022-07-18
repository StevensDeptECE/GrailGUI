#pragma once

#include "opengl/Shape.hh"

class Rectangle : public Shape {
 private:
   glm::vec4 fillColor;

 public:
  Rectangle(Canvas* c, float x, float y, float width, float height, const glm::vec4& color);

  void init() override;
  void update() override;
  void render(glm::mat4& trans) override;
 
};
