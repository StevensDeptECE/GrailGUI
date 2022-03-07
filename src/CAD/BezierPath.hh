#pragma once
#include "opengl/GrailGUI.hh"
#include "./Vec3d.hh"
//#include "./Curve.hh"
#include <vector>
#include "opengl/Shape2D.hh"


//#include "src/CAD/line.hh"

//public Shape2D 

class BezierPath : public Shape2D{
 public: //protected
    std::vector<Vec3D> points; //drawing points
    //std::vector<Vec3D> point; //4 user inputted points
    //individual points p1, p2, p3, p4
    Vec3D p1, p2, p3, p4;
    float ax, bx, cx, dx;
    float ay, by, cy, dy;
    float az, bz, cz, dz;
    std::vector<float> drawingPoints;
  
  // :Shape2D(c, p1.x, p1.y, s)
  public:
    BezierPath(Vec3D p1, Vec3D p2, Vec3D p3, Vec3D p4, Canvas* c, Style* s) :Shape2D(c, p1.x, p1.y, s),
    p1(p1), p2(p2), p3(p3), p4(p4) {
      this->getPoints();
      this->drawingPoints = this->unwrap(this->points);
    }

  void getEquationX(double t, const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3);
  void getEquationY(double t, const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3);
  void getEquationZ(double t, const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3);
  float x(double t);
  float y(double t);
  float z(double t);
  //Vec3D getPoint(double step);
  void getPoints();
  std::vector<float> unwrap(std::vector<Vec3D> x);
  
  void init();
  void render();
  

  //Vec3D getPoint(double step) override {return Vec3D(0,0,0);}
  //Vec3D getTangent(double step) override {return Vec3D(0,0,0);}
  //Matrix3D getMatrix(double step);

};
