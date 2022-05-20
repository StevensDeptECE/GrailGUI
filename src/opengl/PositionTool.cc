#include "opengl/PositionTool.hh"

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

void PositionTool::init() {
  for (int i = 0; i < xCord.size(); i++) {
    m->drawCircle(xCord.at(i), yCord.at(i), radius.at(i), 1, grail::blue);
  }
}

void PositionTool::addNewCord(float newX, float newY, float newR) {
  xCord.push_back(newX);
  yCord.push_back(newY);
  radius.push_back(newR);
}

void PositionTool::update(float newX, float newY, uint32_t position) {
  xCord.at(position) = newX;
  yCord.at(position) = newY;
}
