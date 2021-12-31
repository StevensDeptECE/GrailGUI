#pragma once
//#include "opengl/GrailGUI.hh"
//#include "CAD/Curve.hh"
#include "CAD/Vec3d.hh"
#include <vector>

//using namespace std;

/**
Represent -- 
**/
class Circle : public Vec3D {
  protected:
    double radius;
     Vec3D center;
     Vec3D point; 
     Vec3D p, u, v;
   
    public:
      Circle(double radius, Vec3D center, Vec3D point) {
        this->radius = radius;
        this->center = center;
        this->point = point;
      }
    
    double getRadius() const {return radius;}
    void compute();
    Vec3D getPoint(double step);
    //Vec3D getTangent(double step);

};