#include "CAD/RectangularPrism.hh"
// #include "opengl/GLMath.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"

RectangularPrism::RectangularPrism(Canvas* c, uint32_t length, uint32_t width, uint32_t height, uint32_t x, uint32_t y, uint32_t z) : Shape(c), length(length), width(width), height(height), x(x), y(y), z(z){
}
// RectangularPrism::~RectangularPrism(){
// }

inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  // ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
  // ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
}

void RectangularPrism::init(){
  //set vertex values
  uint32_t length_half = length / 2;
  uint32_t width_half = width / 2;
  vert.push_back(x + length_half); vert.push_back(y - width_half); vert.push_back(z); //0
  vert.push_back(x - length_half); vert.push_back(y - width_half); vert.push_back(z); //1
  vert.push_back(x - length_half); vert.push_back(y + width_half); vert.push_back(z); //2
  vert.push_back(x + length_half); vert.push_back(y + width_half); vert.push_back(z); //3
  vert.push_back(x + length_half); vert.push_back(y - width_half); vert.push_back(z + height); //4
  vert.push_back(x - length_half); vert.push_back(y - width_half); vert.push_back(z + height); //5
  vert.push_back(x - length_half); vert.push_back(y + width_half); vert.push_back(z + height); //6
  vert.push_back(x + length_half); vert.push_back(y + width_half); vert.push_back(z + height); //7

  //bottom face
  ind.push_back(1); ind.push_back(0); ind.push_back(2);
  ind.push_back(2); ind.push_back(0); ind.push_back(3);

  //top face
  ind.push_back(5); ind.push_back(4); ind.push_back(6);
  ind.push_back(6); ind.push_back(4); ind.push_back(7);

  //front face
  ind.push_back(1); ind.push_back(0); ind.push_back(5);
  ind.push_back(5); ind.push_back(0); ind.push_back(4);

  //back face
  ind.push_back(2); ind.push_back(3); ind.push_back(6);
  ind.push_back(6); ind.push_back(3); ind.push_back(7);

  //side faces
  ind.push_back(0); ind.push_back(3); ind.push_back(4);
  ind.push_back(4); ind.push_back(3); ind.push_back(7);

  ind.push_back(2); ind.push_back(1); ind.push_back(6);
  ind.push_back(6); ind.push_back(1); ind.push_back(5);

  //Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  gen(vbo, vertices); // handle for the buffer object for vertices
  //Define parameter 0 in vertex shader contains 3 floats each (x,y,z)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

 glGenBuffers(1, &elementbuffer);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(uint32_t), &ind[0], GL_STATIC_DRAW);

}

// double RectangularPrism::getVolume(){
//   double volume = length * width * height;
//   return volume;
// }

void RectangularPrism::render(){
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, (void*)0);
}

void RectangularPrism::cleanup(){
}

void RectangularPrism::update(){
}