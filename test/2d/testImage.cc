#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"

using namespace std;
using namespace grail;

/*
  Demonstration of the old way to create a grail graphical program, by
  subclassing GLWin. This is no longer the best way In this case we load an
  image and display it in the window
*/
class TestImage : public GLWin {
 public:
  TestImage() : GLWin(0x000000, 0xCCCCCC, "TestImage") {}

  void update() {}

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();

    Image* img = c->addLayer(new Image(c, 50, 550, 300, 400, 1));
    img->addImage("textures/mars.jpg");
    // overwrites the previously added image
    img->addImage("textures/sun.jpg");
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestImage(), 1000, 1000);
}
