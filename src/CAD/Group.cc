#include "CAD/Group.hh"

void Group::init() {
  for (auto s : shapes) s->init();
}

void Group::render() {
  for (auto s : shapes) s->render();
}
