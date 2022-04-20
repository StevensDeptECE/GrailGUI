#pragma once
#include "opengl/GrailGUI.hh"
#include "opengl/util/Transformation.hh"
#include "opengl/MultiShape3D.hh"
#include "./Vec3d.hh"
#include <vector>
#include "opengl/Shape2D.hh"


/**
Represent -- 
**/
class Helix: public Vec3D, public Shape2D {
  protected:
    std::vector<Vec3D> points;
    Vec3D start;
    double height;
    double radius;
    std::vector<float> drawingPoints;
  
  public:
  // Canvas* c, Style* s
    Helix(Vec3D start, double height, double radius, Canvas* c, Style* s):Shape2D(c, start.x, start.y, s)
      {
      this->start=start;
      this->height=height;
      this->radius=radius;
      this->getPoints();
    }

  void init();
  void render();
  //where is the origin?

  Vec3D getPoint(double step);
  void getPoints();
  
};

