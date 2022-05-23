#include "CAD/Rectangle.hh"

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

Rectangle::Rectangle(Canvas* c, float x, float y, float width, float height,
                     const Style* s)
    : Shape2D(c, x, y, s),
      x1(x),
      y1(y),
      x2(x + width),
      y2(y),
      x3(x),
      y3(y + height),
      x4(x + width),
      y4(y + height) {}  //, x5(x+width), y5(y){}//, x6(x), y6(y+height){}

void Rectangle::init() {
  // Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &x1, GL_DYNAMIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void Rectangle::render() {
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  shader->setVec4("solidColor", style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  // glDrawArrays(GL_LINE_LOOP, 0, 3);
  // glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

// float Rectangle::getArea(){
//   return this->width * this->height;
// }

// float getArea(float base, float height){
//    return (.5 * base * height);
//}

// float getArea(float x, float y,float x2, float y2,float x3, float y3){
// write formula
//}
