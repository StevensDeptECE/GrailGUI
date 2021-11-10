#include "CAD/Curve.cc"
#include "iostream"
#include "CAD/Vec3d.hh"
using namespace std;


int main() {
 // Curve test();
  Curve test = new Curve();
  cout << test << endl;
  Vec3D p1 = new Vec3D(1,2,3)
  test.add(p1);
  cout << test.size() << endl;
  test.resize();
  cout << test << endl;
}

//class TestCurve : public GLWin {