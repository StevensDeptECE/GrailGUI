#define _USE_MATH_DEFINES

#include "CAD/Circle.hh"

#include <math.h>

#include <cmath>

using namespace std;

// https://www.quora.com/A-problem-in-3D-geometry-what-is-the-equation-of-the-circle-see-details#:~:text=The%20equation%20of%20the%20circle%20is%20(x%20%2D%20p)%5E,%5E2%20%3D%20r%5E2.&text=Let%20the%20x%2Dcoordinate%20of,center%20is%20%2D5%2F2.

void Circle::compute() {
  p = normalize(point);
  u = cross(center, p);  // get perpendicular vector
  u = normalize(u);      // normalize to get unit vector
  v = cross(u, p);       // get perpendicular vector
  v = normalize(v);      // normalize to get unit vector
}

Vec3D Circle::getPoint(double step) {
  double t = M_PI * 2 * step;
  compute();
  return center + (u * (radius * cos(t))) + (v * (radius * sin(t)));
}

void Circle::getPoints() {
  for (double i = 0; i <= 1; i += 0.01) {
    points.push_back(getPoint(i));
  }
}

std::vector<float> Circle::unwrap(std::vector<Vec3D> x) {
  std::vector<float> temp(x.size() * 3);
  for (int i = 0, j = 0; i < x.size(); i++) {
    temp[j++] = x[i].x;
    temp[j++] = x[i].y;
    temp[j++] = x[i].z;
  }
  return temp;
}

void Circle::init() {
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

void Circle::render() {
  // for(int i=0; i<drawingPoints.size(); i++){
  //   cout << drawingPoints[i] << " " ;
  // }
  // cout << endl;

  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  shader->setVec4("solidColor", style->getFgColor());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  // cout<< drawingPoints.size()/3 <<endl;
  glDrawArrays(GL_LINE_LOOP, 0, drawingPoints.size() / 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}