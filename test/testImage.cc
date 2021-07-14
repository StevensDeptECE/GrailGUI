#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"

using namespace std;
using namespace grail;

/*
** Before running this test unzip apple.zip in test/textures/image_demo/
** The images inside it are from https://www.youtube.com/watch?v=FtutLA63Cp8
** we did not create them.
*/
class TestImage : public GLWin {
 public:
  TestImage() : GLWin(0x000000, 0xCCCCCC, "TestImage") {}

  MainCanvas *c;

  void update() {}

  void init() {
    c = currentTab()->getMainCanvas();

    Image *img = c->addLayer(
        new Image(c, 100, 100, 300, 300, "textures/image_demo/frame13.png"));
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestImage(), 500, 500);
}
