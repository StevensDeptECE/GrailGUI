#include <numbers>

#include "opengl/AngledMultiText.hh"
#include "opengl/AxisWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class testAxis : public GLWin {
 public:
  testAxis() : GLWin(0x000000, 0xCCCCCC, "TestAxisWidget") {}

  void testLinearAxis(StyledMultiShape2D *gui, MultiText *guiText,
                      const Style *style) {
    LinearAxisWidget axis(gui, guiText, 600, 100, 400, 200, style->f);
    gui->drawRectangle(600, 100, 400, 200, grail::cyan);
    // linear specific
    axis.setBounds(0, 1);
    axis.setTickInterval(0.10);

    // general
    axis.setTickDrawSize(8);
    axis.setShowTicks(true);
    axis.setTitle("Test Title");
    axis.init();
  }

  void testLogAxis(StyledMultiShape2D *gui, MultiText *guiText,
                   const Style *style) {
    LogAxisWidget axis(gui, guiText, 100, 400, 400, 200, style->f);
    gui->drawRectangle(100, 400, 400, 200, grail::green);
    // log specific
    // axis.setNumTicks(3);
    // axis.setScale(2, 3);
    axis.setBounds(1, 10000);
    axis.setTickInterval(10);

    // general
    axis.setTickFormat(2, 0);
    axis.setTickDrawSize(10);
    axis.setTickColor(grail::purple);
    axis.setTitle("Longer Title");
    axis.init();
  }

  void testTextAxis(StyledMultiShape2D *gui, MultiText *guiText,
                    const Style *style) {
    vector<string> labels = {"A", "B", "C", "D", "E", "F"};
    TextAxisWidget axis(gui, guiText, 100, 25, 300, 300, style->f);
    gui->drawRectangle(100, 25, 300, 300, grail::blue);

    // text specific
    axis.setTickLabels(labels);

    // general
    axis.setTitle("Categories");
    axis.setTickDrawSize(7);
    axis.setAxisColor(grail::red);
    axis.setShowTicks(true);
    axis.init();
  }

  void testRotatedAxis(Canvas *c, StyledMultiShape2D *gui, MultiText *guiText,
                       const Style *style) {
    double width = 300;
    double height = 300;
    double x = 700;
    double y = 400;

    StyledMultiShape2D *rot90 = c->addLayer(new StyledMultiShape2D(
        c, style, numbers::pi / 2, x - width, y + height));
    AngledMultiText *t90 = c->addLayer(new AngledMultiText(c, style, 0, x, y));
    LinearAxisWidget axis(rot90, t90, 0, 0, width, height, style->f);
    // LogAxisWidget axis(rot90, t90, 0, 0, width, height);
    gui->drawRectangle(x, y, width, height, grail::darkred);

    // linear specific
    axis.setBounds(0, 100);
    // log
    // axis.setBounds(1, 10000);
    axis.setTickInterval(10);

    // general
    axis.setTickDrawSize(8);
    axis.setShowTicks(true);
    axis.setIsVert(true);
    axis.setTitle("Test Title");
    axis.init();
  }

  void init() {
    const Style *s =
        new Style("TIMES", 12, 1, 0, 0, 0,  // Sets a black background (unused)
                  0, 0, 0);                 // Sets black foreground text

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();
    MultiText *guiText = c->addLayer(new MultiText(c, s));

    testLinearAxis(gui, guiText, s);
    testLogAxis(gui, guiText, s);
    testTextAxis(gui, guiText, s);
    testRotatedAxis(c, gui, guiText, s);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new testAxis(), 1000, 1000);
}
