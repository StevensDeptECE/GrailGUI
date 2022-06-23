#include <iostream>

#include "../src/util/JulianDate.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class DateDemo : public GLWin {
 public:
  DateDemo() : GLWin(0x000000, 0xCCCCCC, "Test Date") {}

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    MultiText* guiText = c->getGuiText();

    JulianDate a(12, 1, 12);
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new DateDemo()); }
