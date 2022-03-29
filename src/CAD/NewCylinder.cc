#include <unistd.h>
#include "CAD/Cylinder.hh"
#include "opengl/GLMath.hh"
#include "CAD/Vec3d.hh"
#include <cmath>
#include <numbers>
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include <GL/glut.h>

using namespace std::numbers;

// Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t r2, uint32_t res) : height(height), r1(r1), r2(r2), res(res){
// }
Cylinder::Cylinder(uint32_t height, uint32_t radius) : height(height), radius(radius){
}
// Cylinder::~Cylinder(){
// }

void Cylinder::init(){
  
}

void Cylinder::render(){
  GLfloat x, y, angle = 0.0;
  //bottom circle and sides
  glColor3ub(255, 0, 0);
  glBegin(GL_QUAD_STRIP);
  while(angle < 2 * pi){
    x = radius * cos(angle);
    y = radius * sin(angle);
    glVertex3f(x, y, height);
    glVertex3f(x, y, 0);
    angle = angle + 0.1;
  }
  glVertex3f(radius, 0.0, height);
  glVertex3f(radius, 0.0, 0.0);
  glEnd();
  //top circle
  glColor3ub(255, 0, 0);
  glBegin(GL_POLYGON);
  angle = 0.0;
  while(angle < 2 * pi){
    x = radius * cos(angle);
    y = radius * sin(angle);
    glVertex3f(x, y, height);
  }
  glVertex3f(radius, 0.0, height);
  glEnd();
}


void Cylinder::cleanup(){
}

void Cylinder::update(){
}