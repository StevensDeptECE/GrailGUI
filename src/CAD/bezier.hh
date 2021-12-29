#pragma once
//#include "opengl/GrailGUI.hh"
#include "./Vec3d.hh"
#include "./Curve.hh"
#include <vector>

//#include "src/CAD/line.hh"


class Bezier : public Curve {
 protected:
    //std::vector<Curve> segments;
    std::vector<Vec3D> point;
    float ax, bx, cx, dx;
    float ay, by, cy, dy;
    float az, bz, cz, dz;
  
  public:
    Bezier(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4) {
      point.push_back(p1);
      point.push_back(p2);
      point.push_back(p3);
      point.push_back(p4);
    }

  void getEquationX(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3);
  void getEquationY(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3);
  void getEquationZ(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3);
  float x(float t) const;
  float y(float t) const;
  float z(float t) const;

  

  Vec3D getPoint(double step) override {return Vec3D(0,0,0);}
  Vec3D getTangent(double step) override {return Vec3D(0,0,0);}
  //Matrix3D getMatrix(double step);

};
