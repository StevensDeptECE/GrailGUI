#define _USE_MATH_DEFINES
#include "CAD/Curve.hh"

#include <cmath>

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

using namespace std;

void Curve::add(const Vec3D& p) { points.push_back(p); }

void Curve::compute() {
  p = normalize(point);
  u = cross(center, p);  // get perpendicular vector
  u = normalize(u);      // normalize to get unit vector
  v = cross(u, p);       // get perpendicular vector
  v = normalize(v);      // normalize to get unit vector
}

Vec3D Curve::getPoint(double step) {
  double t = M_PI * 2 * step;
  compute();
  cout << "step: " << step << ", t: " << t << ", point: "
       << center + (u * (radius * cos(t))) + (v * (radius * sin(t))) << endl;
  return center + (u * (radius * cos(t))) + (v * (radius * sin(t)));
}

void Curve::getPoints() {
  for (double i = 0; i <= 0.5; i += 0.1) {
    // cout<< this->getPoint(i) << endl;
    points.push_back(this->getPoint(i));
  }
}

// unwrap points to be a 1d array for drawing
std::vector<float> unwrap(std::vector<Vec3D> x) {
  std::vector<float> temp(x.size() * 3);
  for (int i = 0, j = 0; i < x.size(); i++) {
    temp[j++] = x[i].x;
    temp[j++] = x[i].y;
    temp[j++] = x[i].z;  // 0, when z != 0 doesn't work
  }
  return temp;
}

void Curve::init() {
  // create VAO to hold shapes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // create VBO to hold vertex points
  drawingPoints = unwrap(points);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), &drawingPoints[0],
               GL_STATIC_DRAW);

  // describe in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Curve::render(glm::mat4& trans) {
  // for(int i=0; i<drawingPoints.size(); i++){
  //   cout << drawingPoints[i] << " " ;
  // }
  // cout << endl ;

  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  shader->setVec4("solidColor", style->getFgColor());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size() / 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

// #if 0
// void Curve::getMatrix(double step) const {
//   Vector point = getPoint(step);
//   if (point == Vec3D(0,0,0)){ //if point[0]=0 && point[1]=0 && point[2]=0
//     point = new Vector(-0.01, 0.01, 0);
//   }
//   Vector zaxis = getTangent(step);
//   zaxis = normalize(zaxis);

//   Vector yaxis, xaxis;
//   if (zaxis == Vector(0,0,1)){
//     Vector yaxis(0,1,0);
//   }
//   else if (zaxis === Vector(0,0,-1)){
//     Vector yaxis(0,-1,0);
//   }
//   else {
//     yaxis = cross(point, zaxis);
//     yaxis = normalize(yaxis);
//   }
//   if (zaxis == Vector(0,0,1)){
//     Vector xaxis(1,0,0);
//   }
//   else if (zaxis === Vector(0,0,-1)){
//     Vector xaxis(-1,0,0);
//   }
//   else {
//     xaxis = cross(yaxis, zaxis);
//     xaxis = normalize(xaxis);
//   }
//   return new Transformation(xaxis, yaxis, zaxis, point);
// }

// #endif

// int main(){
//   Vec3D a(1,1,1);
//   Vec3D b(2,3,4);

//   Curve c1(a, b);
//   c1.getPoints();
//   cout << "points: " << c1 << endl;
// }