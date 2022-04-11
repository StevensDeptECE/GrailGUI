#include "CAD/RectangularPrism.hh"
// #include "opengl/GLMath.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include "opengl/Style.hh"

RectangularPrism::RectangularPrism(Canvas* c, const Style *s, float length, float width, float height, float x, float y, float z) : Shape(c),style(s), length(length), width(width), height(height), x(x), y(y), z(z){
}
RectangularPrism::~RectangularPrism(){
}
inline void RectangularPrism::addVert(float x, float y, float z) {
  vert.push_back(x);
  vert.push_back(y);
  vert.push_back(z);
}
inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
  ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
}
inline void RectangularPrism::addTri(uint32_t i1, uint32_t i2, uint32_t i3) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
}

void RectangularPrism::init(){
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  //Back Face
  addVert(x, y, z);
  addVert(x + width, y, z);
  addVert(x + width, y + height, z);
  
  addVert(x, y, z);
  addVert(x + width, y + height, z);
  addVert(x, y + height, z);

  //Front face
  addVert(x, y, z + length);
  addVert(x + width, y, z + length);
  addVert(x + width, y + height, z + length);
  
  addVert(x, y, z + length);
  addVert(x + width, y + height, z + length);
  addVert(x, y + height, z + length);



  //Right Face
  addVert(x + width, y, z);
  addVert(x + width, y, z + length);
  addVert(x + width, y + height, z + length);

  addVert(x + width, y, z);
  addVert(x + width, y + height, z + length);
  addVert(x + width, y + height, z);

  //Left Face
  addVert(x, y, z + length);
  addVert(x, y, z);
  addVert(x, y + height, z);
  
  addVert(x, y, z + length);
  addVert(x, y + height, z);
  addVert(x, y + height, z + length);

  //Top Face
  addVert(x, y + height, z);
  addVert(x + width, y + height, z);
  addVert(x + width, y + height, z + length);

  addVert(x, y + height, z);
  addVert(x + width, y + height, z + length);
  addVert(x, y + height, z + length);

  //bottom face
  addVert(x, y, z);
  addVert(x + width, y, z);
  addVert(x + width, y, z + length);

  addVert(x, y, z);
  addVert(x + width, y, z + length);
  addVert(x, y, z + length);



  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert.size(), &vert[0], GL_STATIC_DRAW);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*ind.size(),&ind[0],GL_STATIC_DRAW);

}

void RectangularPrism::render(){
  const Shader* shader = Shader::useShader(style->getShaderIndex());
  //shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_LINES, 0, vert.size());
  //glDrawArrays(GL_TRIANGLES, 0, vert.size());
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  // glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void RectangularPrism::cleanup(){
}

void RectangularPrism::update(){
}
