#include "opengl/Group.hh"

void Group::init() {
  for (auto s : shapes) s->init();
}

void Group::render(glm::mat4& proj) {
  glm::mat4 child = trans * proj;
  for (int i = 0; i < shapes.size(); i++) shapes[i]->render(child);
}