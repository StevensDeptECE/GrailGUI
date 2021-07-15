#pragma once
#include <glm/glm.hpp>
#include <iostream>
//#include <glm/gtc/matrix_transform.hpp>
//
#include "opengl/MultiText.hh"

class AngledMultiText : public MultiText {
 private:
  glm::mat4 transform;

 public:
  AngledMultiText(Canvas *c, const Style *s, float angle, float x, float y)
      : MultiText(c, s), transform(1.0f) {
    transform = glm::translate(transform, glm::vec3(x, y, 0));
    transform = glm::rotate(transform, angle, glm::vec3(0, 0, -1));
  }
  // void printTransform() {
  //   const float *m = &transform[0][0];
  //   std::cout << m[0] << '\t' << m[1] << '\t' << m[2] << '\t' << m[3] << '\n'
  //             << m[4] << '\t' << m[5] << '\t' << m[6] << '\t' << m[7] << '\n'
  //             << m[8] << '\t' << m[9] << '\t' << m[10] << '\t' << m[11] <<
  //             '\n'
  //             << m[12] << '\t' << m[13] << '\t' << m[14] << '\t' << m[15]
  //             << '\n';
  // }
  void render() override;
};
