#define _USE_MATH_DEFINES
// #include "glad/glad.h"
// #include "opengl/Shader.hh"
// #include "opengl/GLWin.hh"
// #include "opengl/Canvas.hh"
#include <cmath>
#include "./Helix.hh" //why does "CAD/Helix.hh" not work?


using namespace std;

// x = r*cos(t)
// y = r*sin(t)
// z = h*t

// r = radius
// h = height
// t -> 0-1


Vec3D Helix::getPoint(double step){
  double x = radius*cos(step);
  double y = radius*sin(step);
  double z = height*step;
  return Vec3D (x, y, z);
}


void Helix::getPoints(){
  cout<< "start, height, radius: " << start << ", "<< height << ", "<< radius << endl;
  for(double i=0; i<=1; i+=0.01){
      cout<< this->getPoint(i) << endl;
      points.push_back(this->getPoint(i));
  }
}

void Helix::init(){
  //create VAO to hold shapes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  //create VBO to hold vertex points
  drawingPoints = unwrap(points);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), &drawingPoints[0], GL_STATIC_DRAW);

  //describe in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Helix::render(){
  for(int i=0; i<drawingPoints.size(); i++){
    cout << drawingPoints[i] << " " ;
  }
  cout << endl ;

  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_LINE_STRIP, 0, 6);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}

// int main(){
//   Vec3D start(0,0,0);
//   Helix test(start, 2, 1);
//   //cout << "points: " << points << endl;
// }