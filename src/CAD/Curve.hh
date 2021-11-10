#pragma once
#include "opengl/GrailGUI.hh"
#include "opengl/util/Transformation.hh"
#include "CAD/Vec3d.hh"

/**
Represent -- 
**/
class Curve {
  protected:
    std::vector<Vec3D> points;
  
  public:
    Curve() {
      points.reserve(2);
    }

  void add(const Vec3D& p);
  void resize();
  uint32_t size() const {return points.size();}
  virtual Vec3D getPoint(double step) = 0;
  virtual Vec3D getTangent(double step) = 0; //virtual replaces "abstract" from java
  //TODO: Transformation getMatrix(double step); //implement Transformation/import for now but function/constructor is different
};

