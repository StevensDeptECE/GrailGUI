#include <numbers>

#include "opengl/AngledMultiText.hh"
#include "opengl/AxisWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class testAxis : public GLWin {
 public:
  testAxis() : GLWin(0x000000, 0xCCCCCC, "TestAxisWidget") {}

  void testTextAxis(StyledMultiShape2D* gui, MultiText* guiText,
                    const Style* style) {}

  void testRotatedAxis(Canvas* c, StyledMultiShape2D* gui, MultiText* guiText,
                       const Style* style) {}

  void init() {
    const Style* s =
        new Style("TIMES", 12, 1, 0, 0, 0,  // Sets a black background (unused)
                  0, 0, 0);                 // Sets black foreground text

    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* m = c->getGui();
    MultiText* t = c->addLayer(new MultiText(c, s));

    // testing a horizontal linear axis widget
    LinearAxisWidget linear(m, t, 600, 100, 400, 200);
    m->drawRectangle(600, 100, 400, 200, grail::cyan);
    // linear specific
    linear.setBounds(0, 1);
    linear.setTickInterval(0.10);
    // general
    linear.setTickDrawSize(8);
    linear.setShowTicks(true);
    linear.setTitle("Test Title");
    linear.init();

    // testing a horizontal log axis widget
    LogAxisWidget log(m, t, 100, 400, 400, 200);
    m->drawRectangle(100, 400, 400, 200, grail::green);
    // log specific
    log.setBounds(1, 10000);
    log.setTickInterval(10);

    // general
    log.setTickFormat(2, 0);
    log.setTickDrawSize(10);
    log.setTickColor(grail::purple);
    log.setTitle("Longer Title");
    log.init();

    // testing a horizontal text axis
    vector<string> labels = {"A", "B", "C", "D", "E", "F"};
    TextAxisWidget text(m, t, 100, 25, 300, 300);
    m->drawRectangle(100, 25, 300, 300, grail::blue);

    // text specific
    text.setTickLabels(labels);

    // general
    text.setTitle("Categories");
    text.setTickDrawSize(7);
    text.setAxisColor(grail::red);
    text.setShowTicks(true);
    text.init();

    // vertical linear axis
    double width = 300;
    double height = 300;
    double x = 700;
    double y = 400;

    StyledMultiShape2D* rot90 = c->addLayer(
        new StyledMultiShape2D(c, s, numbers::pi / 2, x - width, y + height));
    AngledMultiText* t90 = c->addLayer(new AngledMultiText(c, s, 0, x, y));
    LinearAxisWidget vertical(rot90, t90, 0, 0, width, height);
    // LogAxisWidget vertical(rot90, t90, 0, 0, width, height);
    m->drawRectangle(x, y, width, height, grail::darkred);

    // linear specific
    vertical.setBounds(0, 100);
    // log
    // vertical.setBounds(1, 10000);
    vertical.setTickInterval(10);

    // general
    vertical.setTickDrawSize(8);
    vertical.setShowTicks(true);
    vertical.setIsVert(true);
    vertical.setTitle("Test Title");
    vertical.init();
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new testAxis(), 1000, 1000);
}
