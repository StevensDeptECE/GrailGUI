#include "CAD/Bezier.hh"
#include "CAD/Vec3d.hh"
//#include "CAD/Matrix3d.hh"

using namespace std;

double slope(Vec3D p1, Vec3D p2){ //copying large objects (slow) const Vec3D& p1, const Vec3D& p2
  return (p2.y - p1.y)/(p2.x - p1.x);
}
void Bezier::getEquationX(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
  dx = p0.x;
  cx = slope(p0, p1);
  bx = -3*(p3.x) + 3*(p0.x) + 2*(slope(p0, p1)) + slope(p2, p3);
  ax = p3.x - p0.x - slope(p0, p1) + 3*(p3.x) - 3*(p0.x) - 2*(slope(p0, p1)) + slope(p2, p3);
}

//pX -> point[0] so no input?
void Bezier::getEquationY(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
  dy = p0.y;
  cy = slope(p0, p1);
  by = -3*(p3.y) + 3*(p0.y) + 2*(slope(p0, p1)) + slope(p2, p3);
  ay = p3.y - p0.y - slope(p0, p1) + 3*(p3.y) - 3*(p0.y) - 2*(slope(p0, p1)) + slope(p2, p3);
}

void Bezier::getEquationZ(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
  dz = p0.z;
  cz = slope(p0, p1);
  bz = -3*(p3.z) + 3*(p0.z) + 2*(slope(p0, p1)) + slope(p2, p3);
  az = p3.z - p0.z - slope(p0, p1) + 3*(p3.z) - 3*(p0.z) - 2*(slope(p0, p1)) + slope(p2, p3);
}

float Bezier::x(float t) const {
  return ((t*ax + bx) * t + cx) * t + dx;
}

float Bezier::y(float t) const{
  return ((t*ay + by) * t + cy) * t + dy;
}

float Bezier::z(float t) const{
  return ((t*az + bz) * t + cz) * t + dz;
}

// int main(){
//   // Vec3D a(1,2,3);
//   // Vec3D b(4,5,6);
//   // Vec3D c(7,8,9);
//   // Vec3D d(10,11,12);
//   Vec3D a(2,2,3);
//   Vec3D b(6,2,3);
//   Vec3D c(6,8,3);
//   Vec3D d(10,8,3);

//   Bezier b1(a,b,c,d);
//   b1.getEquationX(a,b,c,d);
//   b1.getEquationY(a,b,c,d);
//   b1.getEquationZ(a,b,c,d);


//   for(double i=0; i<=1; i+=0.2){
//      // cout <<i << endl;
//       cout << b1.x(i) << ", " << b1.y(i) << ", " << b1.z(i) << endl;
//   }
//   //cout << b1.x(1) << b1.y(1) << b1.z(1) << endl;


//   //cout << b1 << endl;
// }
// void Bezier::getPoint(double step){
//   int number = super.size();
//   if (number < 2){
//     return null;
//   }

//   int[] mi = new int[number];
//   mi[0] = 1;
//   mi[1] = 1;
//   for (int i = 3; i <= number; i++){
//     int[] t = new int[i - 1];
//     for (int j=0; j < t.length; j++){
//       t[j] = mi[j];
//     }
//     mi[0] = 1;
//     mi[i - 1] = 1;
//     for (int j = 0; j < i - 2; j++){
//       mi[j + 1] = t[j] + t[j + 1];
//     }
//   }
// }

// void Bezier::getTangent(double step){
//   double deltaStep = 0.0000001;
//   return getPoint(step+deltaStep) - (getpoint(step)) / deltaStep;
// }


// void Bezier::getMatrix(double step){
//   Vec3D point = getPoint(step);
//   if (point.x == 0 && point.y == 0 && point.z == 0){
//     point = new Vec3D(-0.01, 0.01, 0);
//   }
//   Vec3D zaxis = getTangent(step);
//   zaxis = zaxis.normalized();

//   Vec3D yaxis, xaxis;
//   if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == 1){
//     yaxis = new Vec3D(0,1,0);
//   }
//   else if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == -1){
//     yaxis = new Vec3D(0,-1,0);
//   }
//   else {
//     yaxis = point.cross(zaxis);
//     yaxis = yaxis.normalized();
//   }
//   if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == 1){
//     xaxis = new Vec3D(1,0,0);
//   }
//   else if (zaxis.x == 0 && zaxis.y == 0 && zaxis.z == -1){
//     xaxis = new Vec3D(-1,0,0);
//   }
//   else {
//     xaxis = yaxis.cross(zaxis);
//     xaxis = xaxis.normalized();
//   }
//   return new Matrix3D(xaxis, yaxis, zaxis, point);

// }

