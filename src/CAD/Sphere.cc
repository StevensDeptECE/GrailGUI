#include "CAD/Sphere.hh"
#include "opengl/GLMath.hh"
#include "CAD/Vec3d.hh"
#include <cmath>
#include <numbers>
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"

Sphere::Sphere(uint32_t radius, uint32_t x, uint32_t y, uint32_t z) : radius(radius), x(x), y(y), z(z){
}
Sphere::~Sphere(){
}

void Sphere::init(){
  
}

void Sphere::render(){
}

void Sphere::cleanup(){
}