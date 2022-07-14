#define _USE_MATH_DEFINES
#include "./Line.hh"

#include <cmath>

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

using namespace std;

Vec3D Seg::getPoint(double step) {
  double x = start.x + (step * stop.x);
  double y = start.y + (step * stop.y);
  double z = start.y + (step * stop.y);
  return Vec3D(x, y, z);
}

// void Line::getPoints(){
//   //cout<< "start, height, radius: " << start << ", "<< height << ", "<<
//   radius << endl; for(double i=0; i<=1; i+=0.01){
//      // cout<< this->getPoint(i) << endl;
//       points.push_back(this->start+(this->stop*i));
//   }
// }

// unwrap points to be a 1d array for drawing
std::vector<float> unwrap(std::vector<Vec3D> x) {
  std::vector<float> temp(x.size() * 3);
  for (int i = 0, j = 0; i < x.size(); i++) {
    temp[j++] = x[i].x;
    temp[j++] = x[i].y;
    temp[j++] = x[i].z;
  }
  return temp;
}

void Seg::init() {
  // create VAO to hold shapes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // create VBO to hold vertex points
  drawingPoints = unwrap(points);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, drawingPoints.size() * sizeof(float),
               &drawingPoints[0], GL_STATIC_DRAW);

  // describe in shaders
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Seg::render(glm::mat4& trans) {
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  shader->setVec4("solidColor", style->getFgColor());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_LINES, 0, drawingPoints.size() / 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}