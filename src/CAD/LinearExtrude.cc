#include "CAD/LinearExtrude.hh"

#include <cmath>

#include "CAD/Circle2D.hh"
#include "CAD/Gear.hh"
#include "CAD/Line.hh"
#include "CAD/Rectangle.hh"
#include "CAD/Triangle.hh"
#include "CAD/Vec3d.hh"
#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

using namespace std;

// posible shapes: circle2d, gear, rectangle, triangle
void LinearExtrude::draw() {
  cout << "in linear extrude draw" << endl;
  Line* ltest = new Line(this->start, this->stop, c, baseGraphStyle);
}