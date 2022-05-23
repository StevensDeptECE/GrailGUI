#include <iostream>
#include <vector>

#include "CAD/Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/util/Camera.hh"

using namespace std;

class Vec3DTest : public GLWin {
 private:
  Style *baseGraphStyle;

 public:
  Vec3DTest() : GLWin(0x000000, 0xCCCCCC, "Vec3DTest") {}

  void init() {
    Vec3D a(2, 4, 6);
    Vec3D b(1, 2, 3);
    double c = 2;
    cout << "Vector a: " << a << endl;
    cout << "Vector b: " << b << endl;
    cout << "Constant c: " << c << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * c = " << a * c << endl;
    cout << "a / c = " << a / c << endl;
    cout << "dot product of a and b = " << dot(a, b) << endl;
    cout << "cross product of a and b = " << cross(a, b) << endl;
    cout << "midpoint between a and b = " << midpoint(a, b) << endl;
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new Vec3DTest(), 1000, 1000);
}
