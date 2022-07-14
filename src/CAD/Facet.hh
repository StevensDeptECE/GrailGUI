#pragma once
#include <vector>

#include "./Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/Shape2D.hh"
#include "opengl/util/Transformation.hh"
using namespace std;

class Facet : public Vec3D, public Shape2D {
 public:
  std::vector<Vec3D> vertex;
  Canvas* c;
  Style* s;

 public:
  Facet(std::vector<Vec3D> v, Canvas* c, Style* s)
      : Shape2D(c, v[0].x, v[0].y, s) {
    this->vertex = v;
    this->c = c;
    this->s = s;
    this->connect(vertex);
    // for (int i=0; i<v.size(); i++){
    //   cout<< v[i] << endl;
    // }
  }
  // public Vec3D normal();
  // void add(Vec3D v){vertex.push_back(v);};
  //  Vec3D get(int index){return vertex[index];};
  int size() const { return vertex.size(); };
  void connect(std::vector<Vec3D> vertices);
  void init() override;
  void render(glm::mat4& trans) override;
};
