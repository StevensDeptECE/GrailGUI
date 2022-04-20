#define _USE_MATH_DEFINES
// #include "glad/glad.h"
// #include "opengl/Shader.hh"
// #include "opengl/GLWin.hh"
// #include "opengl/Canvas.hh"
#include <cmath>
#include "./Helix.hh"


using namespace std;

// x = r*cos(t)
// y = r*sin(t)
// z = h*t

// r = radius
// h = height
// t -> 0-1


Vec3D Helix::getPoint(double step){
  double x = start.x+radius*cos(2*M_PI*step);
  double y = start.y+radius*sin(2*M_PI*step);
  double z = start.z+height*step;
  return Vec3D (x, y, z);
}


void Helix::getPoints(){
  cout<< "start, height, radius: " << start << ", "<< height << ", "<< radius << endl;
  for(double i=0; i<=1; i+=0.01){
      cout<< this->getPoint(i) << endl;
      points.push_back(this->getPoint(i));
  }
}

//unwrap points to be a 1d array for drawing
// std::vector<float> unwrap(std::vector<Vec3D> x){
//   std::vector<float> temp(x.size()*3);
//   for (int i=0, j=0; i<x.size(); i++){
//     temp[j++] = x[i].x;
//     temp[j++] = x[i].y;
//     temp[j++] = x[i].z;
//   }
//   return temp;
// }


void Helix::init(){
  //create VAO to hold shapes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  //create VBO to hold vertex points
  //drawingPoints = unwrap(points);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, drawingPoints.size() * sizeof(float), &drawingPoints[0], GL_STATIC_DRAW);

  //describe in shaders
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Helix::render(){
  // for(int i=0; i<drawingPoints.size(); i++){
  //   cout << drawingPoints[i] << " " ;
  // }
  // cout << endl;

  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  //cout<< drawingPoints.size()/3 <<endl;
  glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size()/3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}

// int main(){
//   Vec3D start(0,0,0);
//   Helix test(start, 2, 1);
//   //cout << "points: " << points << endl;
// }