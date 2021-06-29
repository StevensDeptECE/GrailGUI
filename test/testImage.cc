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

  MainCanvas* c;

  const int numImages = 18;
  int counter = 2;
  char tmp[35];
  Image* img;
  double startTime;
  double frameTime = 1 / 24.0;

  void update() {
    if (getTime() > startTime + frameTime) {
      sprintf(tmp, "textures/image_demo/frame%d.png", counter);
      const char* filename = tmp;

      img->change(filename);
      // img->init();

      if (counter < numImages) {
        counter++;
      } else {
        counter = 1;
      }
      startTime += frameTime;
    }
  }

  void init() {
    startTime = getTime();
    c = currentTab()->getMainCanvas();
    const Style* s = c->getStyle();

    img = c->addLayer(new Image(c, 100, 100, 300, 300, "textures/image_demo/frame1.png", s));

    // image->addImage(900,500, 100,100, 0,0.5f, 0.25f,0.67f);  //! Logo
    // image->addImage(800,400, 100,100, .375f,.5f, .625f,.7f); //! Face
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestImage(), 500, 500);
}
