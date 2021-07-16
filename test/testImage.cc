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
  Image* img;
  MainCanvas* c;

 public:
  TestImage() : GLWin(0x000000, 0xCCCCCC, "TestImage") {}

  void update() {}

  void init() {
    c = currentTab()->getMainCanvas();

    img = c->addLayer(
        new Image(c, 50, 50, 500, 500, "textures/image_demo/frame13.png", 1));

    Image* img2 = c->addLayer(new Image(c, 50, 550, 300, 400, 1));
    img2->addImage("textures/mars.jpg");
    // overwrites the previously added image
    img2->addImage("textures/sun.jpg");
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestImage(), 1000, 1000);
}
