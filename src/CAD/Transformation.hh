#pragma once

//GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Group.hh"
/*
  wrap a glm::Mat4 and provide all the methods to cleanly transform
  https://open.gl/transformations
*/

class Transformation : public Shape {
private:
  glm::mat4 transform;
  Shape* s;
public:
  /*
    By default create the identity matrix
    1 0 0 0   x
    0 1 0 0   y
    0 0 1 0   z
    0 0 0 1   1
  */
  Transformation(){
    transform = glm::mat4(1.0f);
  }

  void ident() {
    transform = glm::mat4(1.0f);
  }
  
  /*
  m  m  m  m
  m  m  m  m
  m  m  m  m
  +x +y +z 0
  */
  void translate(float x, float y, float z, Shape shape){
    transform = glm::translate(transform,glm::vec3(x,y,z));
    s = shape;
  }
/*
    By default create the identity matrix
    3 0 0 0    x    3x
    0 1 0 0  * y =  0x+1y+0z
    0 0 2 0    z    0x+0y+2z
    0 0 0 1    1
  */
  void scale(float x, float y, float z, Shape shape){
    transform = glm::scale(transform, glm::vec3(x,y,z));
    s = shape;
  }
  void scale(float s) {
    transform = transform = glm::scale(transform, glm::vec3(s,s,s));
  }

/* start with an identity matrix and set to pure rotation as a convenience function
TODO: make this faster by just writing the right numbers? Not a priority
   a b c 0
   d e f 0
   g h i 0
   0 0 0 1
*/
  void setRotate(float angleRad, float xAxis, float yAxis, float zAxis, Shape shape) {
    transform = glm::rotate(glm::mat4(1.0f), angleRad, glm::vec3(xAxis,yAxis,zAxis));
    s = shape;
  }

  void rotate(float angleRad, float xAxis, float yAxis, float zAxis, Shape shape) {
    transform = glm::rotate(transform, angleRad, glm::vec3(xAxis,yAxis,zAxis));
    s = shape;
  }

  const glm::mat4& getTransform() const {
    return transform;
  }

  friend std::ostream& operator <<(std::ostream& s, const Transformation& t) {
    //TODO: I suspect we are printing the matrix transposed
    const float* m = &t.transform[0][0];
    return s <<
     m[0] << '\t' << m[1] << '\t' << m[2] << '\t' << m[3] << '\n' <<
     m[4] << '\t' << m[5] << '\t' << m[6] << '\t' << m[7] << '\n' <<
     m[8] << '\t' << m[9] << '\t' << m[10] << '\t' << m[11] << '\n' <<
     m[12] << '\t' << m[13] << '\t' << m[14] << '\t' << m[15] << '\n';
  }
	void init() override;
	void render() override;
};

