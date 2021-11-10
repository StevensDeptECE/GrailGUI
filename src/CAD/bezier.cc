#include "CAD/bezier.hh"
#include "CAD/Vec3d.hh"
#include "CAD/Matrix3d.hh"

using namespace std;

void Bezier::getPoint(double step){
  int number = super.size(); //where does super come from
  if (number < 2){
    return null;
  }

  int[] mi = new int[number];
  mi[0] = 1;
  mi[1] = 1;
  for (int i = 3; i <= number; i++){
    int[] t = new int[i - 1];
    for (int j=0; j < t.length; j++){
      t[j] = mi[j];
    }
    mi[0] = 1;
    mi[i - 1] = 1;
    for (int j = 0; j < i - 2; j++){
      mi[j + 1] = t[j] + t[j + 1];
    }
  }
}

void Bezier::getTangent(double step){
  double deltaStep = 0.0000001;
  return getPoint(step+deltaStep) - (getpoint(step)) / deltaStep;
}


void Bezier::getMatrix(double step){
  Vec3D point = getPoint(step);
  if (point.x == 0 && point.y == 0 && point.z == 0){
    point = new Vec3D(-0.01, 0.01, 0);
  }
  Vec3D zaxis = getTangent(step);
  zaxis = zaxis.normalized();

  Vec3D yaxis, xaxis;
  if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == 1){
    yaxis = new Vec3D(0,1,0);
  }
  else if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == -1){
    yaxis = new Vec3D(0,-1,0);
  }
  else {
    yaxis = point.cross(zaxis);
    yaxis = yaxis.normalized();
  }
  if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == 1){
    xaxis = new Vec3D(1,0,0);
  }
  else if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == -1){
    xaxis = new Vec3D(-1,0,0);
  }
  else {
    xaxis = yaxis.cross(zaxis);
    xaxis = xaxis.normalized();
  }
  return new Matrix3D(xaxis, yaxis, zaxis, point);

}

