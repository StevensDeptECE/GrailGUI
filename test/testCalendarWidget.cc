#include <numbers>
#include <string>
#include <vector>

#include "opengl/GrailGUI.hh"
#include "opengl/CalendarWidget.hh"

using namespace std;
using namespace grail;

class TestWidgets : public GLWin {
 public:
  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}


  void init() {
    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c->addLayer(new MultiText(c, s));

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    CalendarWidget calendar(gui, guiText, 100, 100, 500, 400, 2021, 8, grail::black, f);
    calendar.circleDate(6, grail::green);
    calendar.circleDate(27, grail::red);
    calendar.circleDate(30, grail::blue);

    calendar.init();
    
  }

};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestWidgets(), 1024, 600);
}
