#pragma once
#include <iostream>

#include "glm/glm.hpp"

class Color {
 private:
  float r, g, b, a;
  static float clamp1(float v) { return v > 1 ? 1 : v; }

 public:
  Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
  operator glm::vec4() const { return *(glm::vec4*)this; }
  Color lighter() const {
    return Color(clamp1(1.1 * r), clamp1(1.1 * g), clamp1(1.1 * b), a);
  }
  Color darker() const {
    return Color(clamp1(0.9 * r), clamp1(0.9 * g), clamp1(0.9 * b), a);
  }
};

namespace grail {
extern glm::vec4 black;
extern glm::vec4 white;
extern glm::vec4 red;
extern glm::vec4 orange;
extern glm::vec4 darkred;
extern glm::vec4 green;
extern glm::vec4 darkgreen;
extern glm::vec4 blue;
extern glm::vec4 darkblue;
extern glm::vec4 lightblue;
extern glm::vec4 lightgrey;
extern glm::vec4 yellow;
extern glm::vec4 gray;
extern glm::vec4 cyan;
extern glm::vec4 pink;
extern glm::vec4 purple;
extern glm::vec4 brown;

inline void disp(const char msg[], glm::vec4 c) {
  std::cerr << msg << '[' << c.r << ", " << c.g << "," << c.b << "]\n";
}
};  // namespace grail
