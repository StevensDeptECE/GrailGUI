#include "opengl/AxisWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class testAxis : public GLWin {
 public:
  testAxis() : GLWin(0x000000, 0xCCCCCC, "TestAxisWidget") {}

  void testLinearAxis(StyledMultiShape2D *gui, MultiText *guiText,
                      const Style *style) {
    LinearAxisWidget axis(gui, guiText, 100, 100, 400, 400);
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
    LogAxisWidget axis(gui, guiText, 100, 200, 400, 200);
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
    TextAxisWidget axis(gui, guiText, 100, 25, 300, 300);
    // text specific
    axis.setTickLabels(labels);

    // general
    axis.setTitle("Categories");
    axis.setTickDrawSize(7);
    axis.setAxisColor(grail::red);
    axis.setShowTicks(true);
    axis.init();
  }

  void testRotatedAxis(Canvas *c, MultiText *guiText, const Style *style) {
    StyledMultiShape2D *rot90 =
        c->addLayer(new StyledMultiShape2D(c, style, M_PI/2, 0, 200));
    LinearAxisWidget axis(rot90, guiText, 0, 0, 100, 100);

    // linear specific
    axis.setBounds(0, 1);
    axis.setTickInterval(0.10);

    // general
    axis.setTickDrawSize(8);
    axis.setShowTicks(true);
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

    // testLinearAxis(gui, guiText, s);
    // testLogAxis(gui, guiText, s);
    // testTextAxis(gui, guiText, s);
    testRotatedAxis(c, guiText, s);
    gui->drawRectangle(100, 100, 100, 100, grail::green);
    gui->drawCircle(100, 100, 3.5, 3, glm::vec4(0, 0, 1, 1));
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new testAxis(), 1024, 600);
}
