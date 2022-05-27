#pragma once
#include <iostream>
#include <vector>

#include "./Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/Shape2D.hh"
#include "opengl/util/Camera.hh"

using namespace std;

// TODO: This should be 3d?
class Extrusion : public Vec3D, public Shape2D {
 public:
  std::vector<Vec3D> points;
  std::vector<Vec3D> fpoints;
  int size;
  int vertices;
  Canvas* c;
  Style* s;

 public:
  // first vec in fpoints has to be the same as the first point in points!
  Extrusion(std::vector<Vec3D> facetPoints, std::vector<Vec3D> points,
            Canvas* c, Style* s)
      : Shape2D(c, points[0].x, points[0].y, s) {
    this->points = points;
    this->fpoints = facetPoints;
    this->size = points.size();
    this->vertices = facetPoints.size();
    this->c = c;
    this->s = s;
    this->extrude();
  }

  void extrude();

  void init();
  void render();
};
