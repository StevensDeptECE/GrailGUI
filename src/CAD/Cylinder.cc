#include "CAD/Cylinder.hh"
#include "opengl/GLMath.hh"
#include <math.h>


Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t r2, uint32_t res) : height(height), r1(r1), r2(r2), res(res){
}
Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t res) : height(height), r1(r1), r2(r1), res(res){
}
Cylinder::~Cylinder(){
}

void Cylinder::init(){
  double chord = 2 * r1 * sin(PI<double>/res);
  rMin = Math.sqrt(r1 * r1 - chord * chord /4);
  vector<vector<double>> topVerts;//Vector of vectors
  vector<double> topVertex;
  topVertex.push_back(0);
  topVertex.push_back(0);
  topVertex.push_back(height);
  topVerts.push_back(topVertex);
  vector<doubld> bottomVertex;
  bottomVertex.push_back(0);
  bottomVertex.push_back(0);
  bottomVertex.push_back(0);
  topVerts.push_back(bottomVertex);
  for(double theta = 0; theta < 2 * PI<double> - (1.0e-7); theta+=2 * PI<double> / res){
    double x = r1 * cos(theta);
    double y = r1 * sin(theta);
    vector<double> firstVertex;
    firstVertex.push_back(x);
    firstVertex.push_back(y);
    firstVertex.push_back(height);
    topVerts.push_back(firstVertex);
    vector<double> secondVertex;
    secondVertex.push_back(x);
    secondVertex.push_back(y);
    secondVertex.push_back(0);
    topVerts.push_back(secondVertex);
  }
  
}

double Cylinder::getVolume(){
  double volume = PI<double> * r1 * r2 * height;
  return volume;
}

void Cylinder::render(){
}

void Cylinder::cleanup(){
}