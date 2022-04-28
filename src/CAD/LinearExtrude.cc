#include "CAD/LinearExtrude.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include <cmath>
#include "CAD/Vec3d.hh"
#include "CAD/Circle2D.hh"
#include "CAD/Gear.hh"
#include "CAD/Rectangle.hh"
#include "CAD/Triangle.hh"
#include "CAD/Line.hh"

using namespace std;

//posible shapes: circle2d, gear, rectangle, triangle
void LinearExtrude::draw(){
  cout<<"in linear extrude draw" << endl;
  Line* ltest = new Line(this->start,this->stop,c,baseGraphStyle);

}