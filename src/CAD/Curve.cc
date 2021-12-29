#include "CAD/Curve.hh"

using namespace std;

void Curve::add(const Vec3D& p){
  points.push_back(p);
}

#if 0
void Curve::getMatrix(double step) const {
  Vector point = getPoint(step);
  if (point == Vector<0,0,0>){ //if point[0]=0 && point[1]=0 && point[2]=0
    point = new Vector(-0.01, 0.01, 0);
  }
  Vector zaxis = getTangent(step);
  zaxis = zaxis.normalized();

  Vector yaxis, xaxis;
  if (zaxis == Vector(0,0,1)){
    Vector yaxis(0,1,0);
  }
  else if (zaxis === Vector(0,0,-1)){
    Vector yaxis(0,-1,0);
  }
  else {
    yaxis = point.cross(zaxis);
    yaxis = yaxis.normalized();
  }
  if (zaxis == Vector(0,0,1)){
    Vector xaxis(1,0,0);
  }
  else if (zaxis === Vector(0,0,-1)){
    Vector xaxis(-1,0,0);
  }
  else {
    xaxis = yaxis.cross(zaxis);
    xaxis = xaxis.normalized();
  }
  return new Transformation(xaxis, yaxis, zaxis, point);
}

#endif

int main(){
        cout << "in curve.cc" << endl;

}