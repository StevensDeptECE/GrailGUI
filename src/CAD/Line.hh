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
class Line: public Vec3D, public Shape2D {
  protected:
    std::vector<Vec3D> points;
    Vec3D start;
    Vec3D stop;
    std::vector<float> drawingPoints;
    Canvas* c;
    Style* s;
  
  public:
    Line(Vec3D start, Vec3D stop, Canvas* c, Style* s):Shape2D(c, start.x, start.y, s)
      {
      this->start=start;
      this->stop=stop;
      points.push_back(start);
      points.push_back(stop);
    }


  void init();
  void render();


  Vec3D getPoint(double step);
//  void getPoints();
  
};

