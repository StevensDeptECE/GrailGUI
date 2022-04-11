#include "CAD/Sphere.hh"
#include "opengl/GLMath.hh"
#include "CAD/Vec3d.hh"
#include <cmath>
#include <numbers>
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include "opengl/Style.hh"

using namespace std::numbers;

Sphere::Sphere(Canvas *c, const Style *s, uint32_t latRes, uint32_t longRes, uint32_t radius) : Shape(c), style(s), latRes(latRes), longRes(longRes), radius(radius){
}
Sphere::~Sphere(){
}
inline void Sphere::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
  ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
}
inline void Sphere::addTri(uint32_t i1, uint32_t i2, uint32_t i3) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
}

void Sphere::init(){
  //create vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // Phi is lattitude from south to north
  // Theta is the angle that goes from 0 to 360 around the sphere
  const double dphi = pi / latRes;
  double phi = -pi / 2 + dphi;
  //north and south


  for (int i = 0; i < latRes - 1; ++i, phi += dphi){
    double z = radius + sin(phi);
    double r2 = radius * cos(phi);
    double theta = 0;
    const double dtheta = 2 * pi / longRes;
    
    for(int j = 0; j < longRes; ++j, theta += dtheta){
      double x = r2 * cos(theta), y = r2 * sin(theta);
      vert.push_back(x);
      vert.push_back(y); 
      vert.push_back(z);
      // std::cout << x << ", " << y << ", " << z << "     ";
    }
    // std::cout << std::endl;
  }
  // vert.push_back(0); vert.push_back(0); vert.push_back(-radius);
  // vert.push_back(0); vert.push_back(0); vert.push_back(radius);

  //rectangles
  // for (int i = 0; i < vert.size() - longRes; i++){
  //   ind.push_back(i + longRes);
  //   ind.push_back(i);
  //   ind.push_back((i % longRes == longRes - 1 ? -longRes : 0) + i + 1);
  //   ind.push_back(i + longRes);
  //   ind.push_back((i % longRes == longRes - 1 ? -longRes : 0) + i + 1); 
  //   ind.push_back((i % longRes == longRes - 1 ? -longRes : 0) + i + longRes + 1);
  // }
  for (int j = 0, row = 0; j < latRes - 1; ++j, row += longRes) {
    for (int i = longRes-1; i > 0; --i)
      addRect(row+i-1, row+i, row+i+longRes, row+i-1+longRes);
    addRect(row+longRes-1, row, row+longRes, row + longRes + longRes - 1);
  }
  const uint32_t S = latRes * longRes;
  const uint32_t N = S + 1;
  for (int i = 1; i < longRes; i++)
    addTri(S, i-1, i);
  addTri(S, longRes-1, 0);

  // const uint32_t NORTHEDGE = (latRes-1) * longRes;
  // for (int i = 1; i < longRes; i++)
  //   addTri(N, NORTHEDGE+i-1, NORTHEDGE+i);
  // addTri(N, NORTHEDGE+longRes-1, NORTHEDGE);
  
  
  //Vertices
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert.size(), &vert[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  //Indices
  // glGenBuffers(1, &pbo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*ind.size(), &ind[0], GL_STATIC_DRAW);

  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*ind.size(),&ind[0],GL_STATIC_DRAW);

}

void Sphere::render(){
  const Shader* shader = Shader::useShader(style->getShaderIndex());
  //shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glLineWidth(style->getLineWidth());
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);
  //glDrawArrays(GL_TRIANGLES, 0, vert.size());

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Sphere::cleanup(){
}

void Sphere::update(){
}