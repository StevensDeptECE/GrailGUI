#include "CAD/RectangularPrism.hh"
#include "opengl/GLMath.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"

RectangularPrism::RectangularPrism(uint32_t length, uint32_t width, uint32_t height) : length(length), width(width), height(height){
}
RectangularPrism::~RectangularPrism(){
}

inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  // ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
  // ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
}

void RectangularPrism::init(){
  //set vertex values
  vert.push_back(0); vert.push_back(0); vert.push_back(0); //0
  vert.push_back(0); vert.push_back(width); vert.push_back(0); //1
  vert.push_back(length); vert.push_back(width); vert.push_back(0); //2
  vert.push_back(length); vert.push_back(0); vert.push_back(0); //3
  vert.push_back(0); vert.push_back(0); vert.push_back(height); //4
  vert.push_back(0); vert.push_back(width); vert.push_back(height); //5
  vert.push_back(length); vert.push_back(width); vert.push_back(height); //6
  vert.push_back(length); vert.push_back(0); vert.push_back(height); //7

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