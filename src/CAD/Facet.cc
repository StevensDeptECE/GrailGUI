// #define _USE_MATH_DEFINES
// #include "glad/glad.h"
// #include "opengl/Shader.hh"
// #include "opengl/GLWin.hh"
// #include "opengl/Canvas.hh"
// #include <cmath>
#include "CAD/Facet.hh"

#include "CAD/Line.hh"

using namespace std;

// draw a line connecting the points
void Facet::connect(
    std::vector<Vec3D> vertices) {  // make it const reference so its not a copy
  int size = this->size();          // get the last index of v
  Vec3D stop = vertices[size];  // set stop vector to be the last vertex of v
  for (int i = 0; i < size; i++) {
    if (i == size - 1) {  // if at the last point connect it to the first
      // cout<< vertices[i] << ", " << vertices[0] << endl;
      Seg* l = new Seg(vertices[i], vertices[0], this->c, this->s);
      c->addLayer(l);
    } else {  // else connect the point at i to the next one
      // cout<< vertices[i] << ", " << vertices[i+1] << endl;
      Seg* l = new Seg(vertices[i], vertices[i + 1], this->c, this->s);
      c->addLayer(l);
    }
  }
}

void Facet::init() {}

void Facet::render(glm::mat4& trans) {}

// int main(){
//   cout << "in main" << endl;
//   return 0;
// }
