#include "CAD/Cylinder.hh"
#include "opengl/GLMath.hh"

Cylinder::Cylinder(uint32_t height, uint32_t r1, uint32_t r2) : height(height), r1(r1), r2(r2){
}
Cylinder::Cylinder(uint32_t height, uint32_t r1) : height(height), r1(r1), r2(r1){
}
Cylinder::~Cylinder(){
}

void Cylinder::init(){

}

double Cylinder::getVolume(){
  double volume = PI<double> * r1 * r2 * height;
  return volume;
}

void Cylinder::render(){
}

void Cylinder::cleanup(){
}