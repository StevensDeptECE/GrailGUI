#pragma once
#include "opengl/GrailGUI.hh"
#include "src/CAD/Vec3d.hh"
#include "src/CAD/Curve.hh"
//#include "src/CAD/line.hh"


class Bezier extends Curve {
 protected:
    std::vector<Curve> segments;
  
  public:
    Bezier() {
    }

  Vec3D getPoint(double step);
  Vec3D getTangent(double step);
  //Matrix3D getMatrix(double step);

}
