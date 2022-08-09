#pragma once

#include <glm/glm.hpp>

#include "opengl/Shape.hh"
#include "util/DynArray.hh"

class Group : Shape {
 private:
  glm::mat4 trans;
  std::vector<Shape*> shapes;

 public:
  Group();
  ~Group();
  void add(Shape* s) { shapes.push_back(s); }

  void init() override;
  void render(glm::mat4& proj) override;
};
