#include "CAD/Cylinder.hh"
#include "opengl/GLMath.hh"
#include "CAD/Vec3d.hh"
#include <math.h>
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"


Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t r2, uint32_t res) : height(height), r1(r1), r2(r2), res(res){
}
Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t res) : height(height), r1(r1), res(res){
}
Cylinder::~Cylinder(){
}

void Cylinder::init(){
  double chord = 2 * r1 * sin(PI<double>/res);
  rMin = Math.sqrt(r1 * r1 - chord * chord /4);
  vector<Vec3D> topVerts;//Vector of vectors
  Vec3D topVertex(0, 0, height);
  topVerts.push_back(topVertex);
  Vec3D bottomVertex(0, 0, 0);
  topVerts.push_back(bottomVertex);
  for(double theta = 0; theta < 2 * PI<double> - (1.0e-7); theta+=2 * PI<double> / res){
    double x = r1 * cos(theta);
    double y = r1 * sin(theta);
    Vec3D firstVertex(x, y, height);
    topVerts.push_back(firstVertex);
    Vec3D secondVertex(x, y, 0);
    topVerts.push_back(secondVertex);
  }
  //add to a ComplexPoly?
  //Change to glGenBuffer and other grail functions?
  //following code comes from triangle, but modified to how I think it might need to be for cylinder
  //probably incorrect
  // glGenVertexArrays(3, &vao);
  // glBindVertexArray(vao);
  // glGenBuffers(3, &vbo);
  // glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(uint32_t), &r1, GL_DYNAMIC_DRAW);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(uint32_t), (void*)0);
}

double Cylinder::getVolume(){
  double volume = PI<double> * r1 * r2 * height;
  return volume;
}

void Cylinder::render(){
  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttributeArray(0);

  glLineWidth(style->getLineWidth());
  //Use triangle_strip, but not sure how many points would actually be necessary 
  // glDrawArrays(GL_TRIANGLE_STRIP, 0, what I imagine is some large number)
  // For circles of cylinder, one point for center, need more for whole circle
  // glDrawArrays(GL_TRIANGLE_FAN, 0, 1)
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}

void Cylinder::cleanup(){
}