#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>
#include <iostream>
/*
  wrap a glm::Mat4 and provide all the methods to cleanly transform
  https://open.gl/transformations
*/

class Transformation {
 private:
  glm::mat4 transform;

 public:
  // TODO: easy access constant unit vectors
  // const static glm::vec3 X = {1, 0, 0};
  // const static glm::vec3 Y = {0, 1, 0};
  // const static glm::vec3 Z = {0, 0, 1};
  /*
    By default create the identity matrix
    1 0 0 0   x
    0 1 0 0   y
    0 0 1 0   z
    0 0 0 1   1
  */
  Transformation() { transform = glm::mat4(1.0f); }

  void ident() { transform = glm::mat4(1.0f); }

  /*
  m  m  m  m
  m  m  m  m
  m  m  m  m
  +x +y +z 0
  */
  void translate(float x, float y, float z) {
    transform = glm::translate(transform, glm::vec3(x, y, z));
  }
  /*
      By default create the identity matrix
      3 0 0 0    x    3x
      0 1 0 0  * y =  0x+1y+0z
      0 0 2 0    z    0x+0y+2z
      0 0 0 1    1
    */
  void scale(float sx, float sy, float sz) {
    transform = glm::scale(transform, glm::vec3(sx, sy, sz));
  }
  void scale(float s) {
    transform = transform = glm::scale(transform, glm::vec3(s, s, s));
  }

  /* start with an identity matrix and set to pure rotation as a convenience
  function
  TODO: make this faster by just writing the right numbers? Not a priority
     a b c 0
     d e f 0
     g h i 0
     0 0 0 1
  */
  void setRotate(float angleRad, float xAxis, float yAxis, float zAxis) {
    transform =
        glm::rotate(glm::mat4(1.0f), angleRad, glm::vec3(xAxis, yAxis, zAxis));
  }

  void rotate(float angleRad, float xAxis, float yAxis, float zAxis) {
    transform =
        glm::rotate(transform, angleRad, glm::vec3(xAxis, yAxis, zAxis));
  }

  void rotateZ(float angleRad) {
    transform = glm::rotate(transform, angleRad, glm::vec3(0, 0, 1));
  }

  const glm::mat4& getTransform() const { return transform; }

  friend std::ostream& operator<<(std::ostream& s, const Transformation& t) {
    // TODO: I suspect we are printing the matrix transposed
    const float* m = &t.transform[0][0];
    return s << m[0] << '\t' << m[1] << '\t' << m[2] << '\t' << m[3] << '\n'
             << m[4] << '\t' << m[5] << '\t' << m[6] << '\t' << m[7] << '\n'
             << m[8] << '\t' << m[9] << '\t' << m[10] << '\t' << m[11] << '\n'
             << m[12] << '\t' << m[13] << '\t' << m[14] << '\t' << m[15]
             << '\n';
  }
  static void dump(glm::mat4& mat) {
    // TODO: I suspect we are printing the matrix transposed
    const float* m = &mat[0][0];
    std::cerr << std::setprecision(7);
    for (int i = 0, c = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++, c++) std::cerr << std::setw(14) << m[c];
      std::cerr << '\n';
    }
  }

  static void apply(glm::mat4& m, double x, double y, double z) {
    std::cerr << "x'=" << (m[0][0] * x + m[1][0] * y + m[2][0] * z + m[3][0])
              << "\ty'=" << (m[0][1] * x + m[1][1] * y + m[2][1] * z + m[3][1])
              << "\tz'=" << (m[0][2] * x + m[1][2] * y + m[2][2] * z + m[3][2])
              << '\n';

    std::cerr << "x'=" << (m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3])
              << "\ty'=" << (m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3])
              << "\tz'=" << (m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3])
              << '\n';
  }
};