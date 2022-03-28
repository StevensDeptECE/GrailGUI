#pragma once
#include "opengl/GrailGUI.hh"
#include "CAD/Vec3d.hh"
#include <vector>
#include "opengl/Shape2D.hh"


//#include "src/CAD/line.hh"

//public Shape2D 

class LinearExtrude{
  public:
  Vec3D start; //start point for extrusion
  Vec3D stop; //stop point for extrusion
  string shape; //shape to extrude
  
  public:
    LinearExtrude(Vec3D start, Vec3D stop, string shape) {
      this->start=start;
      this->stop=stop;
      this->shape=shape;
    }
    
    void draw();

};
