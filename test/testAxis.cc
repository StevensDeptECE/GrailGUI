#include <cmath>
#include "opengl/GrailGUI.hh"

#include "opengl/AxisWidget.hh"

using namespace std;
using namespace grail;

class testWidget : public GLWin {
  public:
    testWidget() : GLWin(0x000000, 0xCCCCCC, "TestAxisWidget") {}

  void testAxis(StyledMultiShape2D *gui, MultiText *guiText, const Style
                *style) {
  LinearAxisWidget axis(gui, guiText, 100, 100, 400, 400);
  axis.setBounds(0, 50);
  axis.setTickInterval(5);
  axis.setTickStyle(style);
  axis.init();
  }

  void init() {
    const Style *s =
      new Style("TIMES", 24, 1, 0, 0, 0,//Sets a black background (unused)
                0, 0, 0);//Sets black foreground text

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c-> addLayer(new MultiText(c, s));


    testAxis(gui, guiText, s);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new testWidget(), 1024, 600);
}
