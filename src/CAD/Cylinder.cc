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
Cylinder::~Cylinder(){
}

inline void Cylinder::getUnitCircleVertices() {
  //This is to generate the unit circle on the XY plane that will be used to help render the cylinder
  float steps = 2 * pi / segments; // How many points around the circle
  float angle;                     // The current angle

  for (int i = 0; i <= segments; ++i) {
    angle = i * steps;
    unitVert.push_back(cos(angle)); // x
    unitVert.push_back(sin(angle)); // y
    unitVert.push_back(0);          // z
  }
}

void Cylinder::init(){
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  uint32_t vertices = segments + 2;

  getUnitCircleVertices();

  // Generate the side vertices twice
  for (int i = 0; i < 2; ++i) {
    float h = height * (i * 2);  
    // Variable k will be used to access the unit circle at the current iteration.
    for (int k = 0; k < segments; k++) {
      // ------ Unit Circle ------ //
      float ux = unitVert[k * 3];
      float uy = unitVert[(k * 3) + 1];
      
      // ------------------------- //

      vert.push_back(ux * radius); // x
      vert.push_back(uy * radius); // y
      vert.push_back(h); // z
    }
  }

  int baseCenterIndex = (int)vert.size() / 3;
  int topCenterIndex = baseCenterIndex + segments + 1;
  std::cout << baseCenterIndex << std::endl;

  int k1 = 0; // 1st vertex at base
  int k2 = segments + 1; // 1st vertex at top

  // indices for side
  for (int i = 0; i < segments; ++i, ++k1, ++k2) {
    ind.push_back(k1);
    ind.push_back(k1 + 1);
    ind.push_back(k2);

    ind.push_back(k2);
    ind.push_back(k1 + 1);
    ind.push_back(k2 + 1);
  }

  // indices for top surface
  // for (int i = 0, k = baseCenterIndex + 1; i < segments; ++i, ++k) {
  //   if(i < segments - 1) {
      
  //     ind.push_back(baseCenterIndex);
  //     ind.push_back(k + 1);
  //     ind.push_back(k);
  //   }
  //   else { // last triangle
  //     ind.push_back(baseCenterIndex);
  //     ind.push_back(baseCenterIndex + 1);
  //     ind.push_back(k);
  //   }
  // }

  for(int i = 0, k = topCenterIndex + 1; i < segments; ++i, ++k)
{
    if(i < segments - 1)
    {
        ind.push_back(topCenterIndex);
        ind.push_back(k);
        ind.push_back(k + 1);
    }
    else // last triangle
    {
        ind.push_back(topCenterIndex);
        ind.push_back(k);
        ind.push_back(topCenterIndex + 1);
    }
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
  glLineWidth(style->getLineWidth());

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

  // glLineWidth(style->getLineWidth());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, (void*)0);

  // glDrawArrays(GL_LINES, 0, vert.size());
  // glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, (void*)0);
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
