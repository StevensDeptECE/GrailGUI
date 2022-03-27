#define _USE_MATH_DEFINES

#include "CAD/Circle.hh"
#include <cmath>
#include <math.h>

using namespace std;

//https://www.quora.com/A-problem-in-3D-geometry-what-is-the-equation-of-the-circle-see-details#:~:text=The%20equation%20of%20the%20circle%20is%20(x%20%2D%20p)%5E,%5E2%20%3D%20r%5E2.&text=Let%20the%20x%2Dcoordinate%20of,center%20is%20%2D5%2F2.

void Circle::compute(){
  p = normalize(point);
  u = cross(center, p); //get perpendicular vector
  u = normalize(u); //normalize to get unit vector
  v = cross(u,p); //get perpendicular vector
  v = normalize(v); //normalize to get unit vector
}

Vec3D Circle::getPoint(double step){
  double t = M_PI * 2 * step;
  compute();
  return center + (u*(radius*cos(t))) + (v*(radius*sin(t)));
}

// int main() {
//   Vec3D a(1,1,1);
//   Vec3D b(2,3,4);

//   Circle c1(1, a, b);
//   c1.compute();
//   for(double i=0; i<=1; i+=0.2){
//       cout<< c1.getPoint(i) << endl;
//   }
//   cout << "in circle1.cc, radius: " << c1.getRadius() << endl;
// }