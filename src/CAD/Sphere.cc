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

void Sphere::init(){
  //create vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // Phi is lattitude from south to north
  // Theta is the angle that goes from 0 to 360 around the sphere
  const double dphi = pi / latRes;
  double phi = -pi / 2 + dphi;

  //band around sphere
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
      cout << x << ", " << y << ", " << z << "     ";
    }
    cout << endl;
  }

  //rectangles
  for (int i = 0; i < vert.size() - longRes; i++){
    ind.push_back(i + longRes);
    ind.push_back(i);
    ind.push_back((i % longRes == longRes - 1 ? -longRes : 0) + i + 1);
    ind.push_back(i + longRes);
    ind.push_back((i % longRes == longRes - 1 ? -longRes : 0) + i + 1); 
    ind.push_back((i % longRes == longRes - 1 ? -longRes : 0) + i + longRes + 1);
  }

  //north and south
  vert.push_back(0); vert.push_back(0); vert.push_back(-radius);
  vert.push_back(0); vert.push_back(0); vert.push_back(radius);

  //Vertices
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert.size(), &vert[0], GL_STATIC_DRAW);

  //Indices
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*ind.size(), &ind[0], GL_STATIC_DRAW);


}

void Sphere::render(){
  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  //glDrawElements(GL_LINES, ind.size(), GL_UNSIGNED_INT, (void*)0);
  glDrawArrays(GL_LINE_STRIP, 0, ind.size()/3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Sphere::cleanup(){
}

void Sphere::update(){
}