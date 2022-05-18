#include "CAD/RectangularPrism.hh"
// #include "opengl/GLMath.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include "opengl/Style.hh"

RectangularPrism::RectangularPrism(Canvas* c, const Style *s, float xsize, float ysize, float zsize, float x, float y, float z) : Shape(c),style(s), xsize(xsize), ysize(ysize), zsize(zsize), x(x), y(y), z(z){
}
RectangularPrism::~RectangularPrism(){
}
inline void RectangularPrism::addVert(float x, float y, float z) {
  vert.push_back(x);
  vert.push_back(y);
  vert.push_back(z);
}
//Best for GL_TRIANGLES
// inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
//   ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
//   ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
// }

//Best for GL_LINES
inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  ind.push_back(i1);  ind.push_back(i2);
  ind.push_back(i2); ind.push_back(i3);
  ind.push_back(i3); ind.push_back(i4);
  ind.push_back(i4); ind.push_back(i1);
}

inline void RectangularPrism::addTri(uint32_t i1, uint32_t i2, uint32_t i3) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
}

void RectangularPrism::init(){
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //Width = xsize 

  addVert(x, y, z); //0
  addVert(x + xsize, y, z); //1
  addVert(x + xsize, y + ysize, z); //2
  addVert(x, y + ysize, z); //3
  addVert(x, y, z + zsize); //4
  addVert(x + xsize, y, z + zsize); //5
  addVert(x + xsize, y + ysize, z + zsize); //6
  addVert(x, y + ysize, z + zsize); //7

  //front
  addRect(0, 1, 2, 3);

  //back
  addRect(4, 5, 6, 7);

  //sides
  addRect(1, 5, 6, 2);

  addRect(4, 0, 3, 7);

  //Top
  addRect(3, 2, 6, 7);

  //Bottom
  addRect(0, 1, 5, 4);


  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert.size(), &vert[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*ind.size(),&ind[0],GL_STATIC_DRAW);

  //use if you want points to be drawn as well
  // glGenBuffers(1,&pbo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*ind.size(),&ind[0],GL_STATIC_DRAW);

}

void RectangularPrism::render(){
  // const Shader* shader = Shader::useShader(style->getShaderIndex());
  // shader->setMat4("projection",*(parentCanvas->getProjection()));
	// shader->setVec4("solidColor",style->getFgColor());
  
  //glDrawArrays(GL_LINE_LOOP, 0, vert.size());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glLineWidth(style->getLineWidth());
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINES, ind.size(), GL_UNSIGNED_INT, 0);
  //for points
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbo);
  //glDrawElements(GL_POINTS, ind.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void RectangularPrism::cleanup(){
}

void RectangularPrism::update(){
}
