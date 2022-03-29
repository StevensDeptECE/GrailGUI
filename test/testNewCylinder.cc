#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include <GL/glut.h>
#include "CAD/NewCylinder.hh"
using namespace std;
using namespace grail;

class TestNewCylinder : public GLWin {
    public:
    TestCylinder() : GLWin(0x000000, 0xCCCCCC, "Test Cylinder") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        NewCylinder* cyl = c->addLayer(new NewCylinder(5.0, 5.0));
  }
};

int main(int argc, char* argv[]) { 
  glutInit(&argc, argv);
  return GLWin::init(new TestNewCylinder(argc, argv)); }