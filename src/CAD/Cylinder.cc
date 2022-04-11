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
#include "opengl/Style.hh"

using namespace std::numbers;

// Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t r2, uint32_t res) : height(height), r1(r1), r2(r2), res(res){
// }
Cylinder::Cylinder(Canvas* c, const Style *s, uint32_t height, uint32_t x, uint32_t y, uint32_t z, uint32_t radius, uint32_t segments) : Shape(c), style(s), height(height), radius(radius), x(x), y(y), z(z), segments(segments){
}
// Cylinder::~Cylinder(){
// }

void Cylinder::init(){
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t vertices = segments + 2;
  
  for(int i=0; i < vertices; i++){
    vert.push_back(x +  (radius * cos( i * (2 * pi / segments))));
    vert.push_back(y +  (radius * sin( i * (2 * pi / segments))));
    vert.push_back(z); //z=0
  }
  for(int i=0; i < vertices; i++){
    vert.push_back(x +  (radius * cos( i * (2 * pi / segments))));
    vert.push_back(y + (radius * sin( i * (2 * pi / segments))));
    vert.push_back(z + height); //z=height
  }


  for(int i=0; i < vertices/2; i++){
    ind.push_back(i);
    ind.push_back(i+1);
    ind.push_back(vertices + i);
    ind.push_back(vertices + i);
    ind.push_back(i+1);
    ind.push_back(vertices + i + 1);
  }

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert.size(), &vert[0], GL_STATIC_DRAW);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*ind.size(),&ind[0],GL_STATIC_DRAW);
}

void Cylinder::render(){
  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  //shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

  // glLineWidth(style->getLineWidth());

  glDrawArrays(GL_LINES, 0, vert.size());
  glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, (void*)0);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}


// double Cylinder::getVolume(){
//   double volume = PI<double> * r1 * r2 * height;
//   return volume;
// }

void Cylinder::cleanup(){
}

void Cylinder::update(){
}
