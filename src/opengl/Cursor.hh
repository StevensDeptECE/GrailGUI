#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

#include "opengl/Shape2D.hh"

class Inputs;
class Cursor : public Shape2D {
 private:
  static std::unordered_map<int, Cursor*> cursors;

  glm::mat4 transform;
  int id;
  float xPos, yPos;
  float height = 20;
  float width = 10;
  float offset = 5;

 public:
  static float getXPos(int id) { return cursors[id]->xPos; }
  static float getYPos(int id) { return cursors[id]->yPos; }
  Cursor(float x, float y, Style* s, int id)
      : Shape2D(x, y, s), id(id), xPos(x), yPos(y) {
    // Triangle
    addPoint(x, y);                                      // Tip
    addPoint(x, y + (height * 2) / 3);                   // UnderTip
    addPoint(x + width, y + (height * 2) / 3 - offset);  // right triangle edge

    // Rect
    addPoint(x + width / 3, y + (height * 2) / 3 - offset);        // TL
    addPoint(x + width / 3 + 5, y + height);                       // BL
    addPoint(x + (width * 2) / 3 + 5, y + height - 3);             // BR
    addPoint(x + (width * 2) / 3, y + (height * 2) / 3 - offset);  // TR

    solidIndices.push_back(0);
    solidIndices.push_back(1);
    solidIndices.push_back(2);

    solidIndices.push_back(3);
    solidIndices.push_back(4);
    solidIndices.push_back(5);

    solidIndices.push_back(3);
    solidIndices.push_back(5);
    solidIndices.push_back(6);

    lineIndices.push_back(0);
    lineIndices.push_back(1);
    lineIndices.push_back(3);
    lineIndices.push_back(4);
    lineIndices.push_back(5);
    lineIndices.push_back(6);
    lineIndices.push_back(2);
    cursors[id] = this;
  }
  void initIndices() {}
  void process_input(Inputs* in, float dt);
  void update();
  void render();
};
