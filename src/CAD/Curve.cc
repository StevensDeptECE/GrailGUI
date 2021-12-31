#include "CAD/Curve.hh"

using namespace std;

void Curve::add(const Vec3D& p){
  points.push_back(p);
}

void Curve::compute(){
  p = normalize(point);
  u = cross(center, p); //get perpendicular vector
  u = normalize(u); //normalize to get unit vector
  v = cross(u,p); //get perpendicular vector
  v = normalize(v); //normalize to get unit vector
}

Vec3D Curve::getPoint(double step){
  double t = M_PI * 2 * step;
  compute();
  return center + (u*(radius*cos(t))) + (v*(radius*sin(t)));
}


void Curve::getPoints(){
  for(double i=0; i<=0.5; i+=0.1){
     // cout<< this->getPoint(i) << endl;
      points.push_back(this->getPoint(i));
  }
}

// #if 0
// void Curve::getMatrix(double step) const {
//   Vector point = getPoint(step);
//   if (point == Vec3D(0,0,0)){ //if point[0]=0 && point[1]=0 && point[2]=0
//     point = new Vector(-0.01, 0.01, 0);
//   }
//   Vector zaxis = getTangent(step);
//   zaxis = normalize(zaxis);

//   Vector yaxis, xaxis;
//   if (zaxis == Vector(0,0,1)){
//     Vector yaxis(0,1,0);
//   }
//   else if (zaxis === Vector(0,0,-1)){
//     Vector yaxis(0,-1,0);
//   }
//   else {
//     yaxis = cross(point, zaxis);
//     yaxis = normalize(yaxis);
//   }
//   if (zaxis == Vector(0,0,1)){
//     Vector xaxis(1,0,0);
//   }
//   else if (zaxis === Vector(0,0,-1)){
//     Vector xaxis(-1,0,0);
//   }
//   else {
//     xaxis = cross(yaxis, zaxis);
//     xaxis = normalize(xaxis);
//   }
//   return new Transformation(xaxis, yaxis, zaxis, point);
// }

// #endif

int main(){
  Vec3D a(1,1,1);
  Vec3D b(2,3,4);

  Curve c1(a, b);
  c1.getPoints();
  cout << "points: " << c1 << endl;
}