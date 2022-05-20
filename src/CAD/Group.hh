#pragma once
#include "opengl/Shape.hh"

class Group : Shape {
 private:
  std::vector<Shape*> shapes;

 public:
  Group();
  ~Group();
  void add(Shape* s) { shapes.push_back(s); }

  void init() override;
  void render() override;
}
