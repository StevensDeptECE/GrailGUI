#include "CAD/Triangle.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"

Triangle::Triangle(Canvas* c, float x, float y, float base, float height, const Style* s) :
    Shape2D(c, x, y, s) , x1(x), y1(y), x2(x+base), y2(y), x3(x), y3(y+height){}

Triangle::Triangle(Canvas* c, float x1, float y1, float x2, float y2,float x3, float y3, const Style* s) :
    Shape2D(c, x1, y1, s) , x1(x1) , y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}

void Triangle::init() {
  // Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &x1, GL_DYNAMIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void Triangle::render(){
  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_TRIANGLES, 0, 3);
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}




//float getArea(float base, float height){
 //   return (.5 * base * height);
//}

//float getArea(float x, float y,float x2, float y2,float x3, float y3){
  //write formula
//}