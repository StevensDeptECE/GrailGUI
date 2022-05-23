#pragma once
#include "opengl/GrailGUI.hh"
//#include "CAD/Curve.hh"
#include <vector>

#include "CAD/Vec3d.hh"
#include "opengl/Shape2D.hh"

using namespace std;

/**
Represent --
**/
class Circle : public Vec3D, public Shape2D {
 protected:
  std::vector<Vec3D> points;  // drawing points
  double radius;
  Vec3D center;
  Vec3D point;
  Vec3D p, u, v;
  std::vector<float> drawingPoints;

 public:
  Circle(double radius, Vec3D center, Vec3D point, Canvas* c, Style* s)
      : Shape2D(c, center.x, center.y, s) {
    this->radius = radius;
    this->center = center;
    this->point = point;
    this->compute();
    this->getPoints();
  }

  double getRadius() const { return radius; }
  void compute();
  Vec3D getPoint(double step);
  void getPoints();
  std::vector<float> unwrap(std::vector<Vec3D> x);
  void init();
  void render();
};

// get the derivative along the path
// take a shape and extrude it around an arbitrary path in space
// what is the orientation
// 3d math, talk to someone in the math department
// 3d geometry? stack overflow