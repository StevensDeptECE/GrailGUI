#pragma once
// #include "opengl/GrailGUI.hh"
// #include "opengl/util/Transformation.hh"
//#include "opengl/MultiShape3D.hh"
#include "./Vec3d.hh"
#include <vector>


/**
Represent -- 
**/
class Curve : public Vec3D {
  protected:
    std::vector<Vec3D> points;
    double radius;
    Vec3D point;
    Vec3D center; 
    Vec3D p, u, v;
   
  
  public:
    Curve(Vec3D p1, Vec3D p2) {
      this->point=p1;
      this->center=midpoint(p1,p2);
      this->radius=distance(p1,p2)/2;
    }

  void add(const Vec3D& p);
  void resize() {return points.reserve(points.size()*2);} //.size or .capacity()
  uint32_t size() const {return points.size();} //size = number of itmes
  uint32_t capacity() const {return points.capacity();} //capacity = all spaces(used or unused)
  void compute();
  Vec3D getPoint(double step);
  void getPoints();
 // virtual Vec3D getTangent(double step) = 0; //virtual replaces "abstract" from java
  
  // Overload the << operator
  friend std::ostream& operator<< (ostream& s, const Curve& c){
    for (int i = 0; i < c.points.size(); i++){
      s << c.points[i] << ",";
    }
    return s;
  }
  
  //Transformation getMatrix(double step); //implement Transformation/import for now but function/constructor is different
};

