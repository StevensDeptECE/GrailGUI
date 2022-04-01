#include "CAD/RectangularPrism.hh"
// #include "opengl/GLMath.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include "opengl/Style.hh"

RectangularPrism::RectangularPrism(Canvas* c, const Style *s, float length, float width, float height, float x, float y, float z) : Shape(c), style(s), length(length), width(width), height(height), x(x), y(y), z(z){
}
RectangularPrism::~RectangularPrism(){
}

inline void RectangularPrism::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  // ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
  // ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
}

void RectangularPrism::init(){
  //set vertex values
  // GLfloat length_half = length / 2;
  // GLfloat width_half = width / 2;
  // vert.push_back(x + length_half); vert.push_back(y - width_half); vert.push_back(z); //0
  // vert.push_back(x - length_half); vert.push_back(y - width_half); vert.push_back(z); //1
  // vert.push_back(x - length_half); vert.push_back(y + width_half); vert.push_back(z); //2
  // vert.push_back(x + length_half); vert.push_back(y + width_half); vert.push_back(z); //3
  // vert.push_back(x + length_half); vert.push_back(y - width_half); vert.push_back(z + height); //4
  // vert.push_back(x - length_half); vert.push_back(y - width_half); vert.push_back(z + height); //5
  // vert.push_back(x - length_half); vert.push_back(y + width_half); vert.push_back(z + height); //6
  // vert.push_back(x + length_half); vert.push_back(y + width_half); vert.push_back(z + height); //7

  add3DPoint(x, y, z);
  add3DPoint(x + width, y, z);
  add3DPoint(x, y + height, z);
  add3DPoint(x + width, y + height, z);
  add3DPoint(x, y, z - length);
  add3DPoint(x + width, y, z - length);
  add3DPoint(x, y + height, z - length);
  add3DPoint(x + width, y + height, z - length);
  add3DPoint(x, y + height, z);
  add3DPoint(x + width, y + height, z);
  add3DPoint(x, y, z);
  add3DPoint(x + width, y, z);
  add3DPoint(x + width, y, z);
  add3DPoint(x + width, y + height, z);

  //bottom face
  ind.push_back(0); ind.push_back(1); ind.push_back(2);
  ind.push_back(1); ind.push_back(2); ind.push_back(3);

  //top face
  ind.push_back(4); ind.push_back(5); ind.push_back(6);
  ind.push_back(5); ind.push_back(6); ind.push_back(7);

  //front face
  ind.push_back(4); ind.push_back(0); ind.push_back(6);
  ind.push_back(0); ind.push_back(6); ind.push_back(2);

  //back face
  ind.push_back(5); ind.push_back(12); ind.push_back(7);
  ind.push_back(12); ind.push_back(7); ind.push_back(13);

  //side faces
  ind.push_back(6); ind.push_back(8); ind.push_back(7);
  ind.push_back(8); ind.push_back(7); ind.push_back(9);

  ind.push_back(4); ind.push_back(10); ind.push_back(5);
  ind.push_back(10); ind.push_back(5); ind.push_back(11);

  // for (int i = 0; i < vert.size(); i++) {
  //   std::cout << vert.size() << std::endl;
  //   if (i % 3 == 0) {
  //     std::cout << std::endl;
  //   }
  //   std::cout << vert[i] << " ";
  // }
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  gen(vbo, vert);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

  gen(sbo, ind);


}

// double RectangularPrism::getVolume(){
//   double volume = length * width * height;
//   return volume;
// }

void RectangularPrism::render(){
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glBindVertexArray(vao);

  glEnableVertexAttribArray(0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  // glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void RectangularPrism::cleanup(){
}

void RectangularPrism::update(){
}
