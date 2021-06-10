#define _USE_MATH_DEFINES

#include <cmath>
#include <string>
#include <vector>

#include "opengl/AngledMultiText.hh"
#include "opengl/BarChartWidget.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/ScrollbarWidget.hh"
#include "opengl/util/Transformation.hh"

using namespace std;
using namespace grail;

class TestWidgets : public GLWin {
 public:
  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}

  void testButton(StyledMultiShape2D *gui, MultiText *guiText) {
    const float boxSize = 100;
    const float drawSize = (boxSize / 4) * 5;

    gui->fillRectangle(boxSize, boxSize, drawSize, drawSize / 5, black);
    gui->fillRectangle(boxSize, boxSize, drawSize / 5, drawSize, black);
    gui->fillRectangle(boxSize, boxSize * 2, drawSize, drawSize / 5, black);
    gui->fillRectangle(boxSize * 2, boxSize * 1.5, drawSize / 5, drawSize * .4,
                       black);
    gui->fillRectangle(boxSize * 1.5, boxSize * 1.5, drawSize * .4,
                       drawSize / 5, black);

    // c->addButton("Hello",100,100,100,100);
    ButtonWidget b(gui, guiText, "hello", 0, 0, 100, 50);
    b.init();
  }

  void testBarChart(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {100, 10, 1000, 150, 10000};
    vector<float> y = {150, 350, 222, 100, 300};

    vector<string> labels = {"bar", "big bar", "other bar", "small bar",
                             "barrr"};
    BarChartWidget chart(gui, guiText, 550, 320, 400, 200, y);
    chart.chart(y, labels, 50);
    chart.setTitle("Title");
    chart.init();

    vector<string> labels2 = {"bar 1", "bar 2", "bar 3", "bar 4", "bar 5"};
    BarChartWidget chart2(gui, guiText, 550, 50, 400, 200, x);
    chart2.setAxisScale(new LogScale());
    chart2.chart(x, labels2, 10);
    chart2.setTitle("Title Log graph");
    chart2.init();
  }

  void testScrollBar(StyledMultiShape2D *gui, MultiText *guiText) {
    const uint32_t scrollBarWidth = 50;
    ScrollbarWidget scrollBar(gui, guiText, getWidth() - scrollBarWidth, 0,
                              scrollBarWidth, getHeight());
    scrollBar.init();
  }

  void testAngleText(StyledMultiShape2D *gui, MultiText *guiText, Canvas *c,
                     const Style *s) {
    const char thing[] = "hello world";
    guiText->add(0, 50, s->f, thing, strlen(thing));
    AngledMultiText *am =
        c->addLayer(new AngledMultiText(c, s, M_PI / 4, 20, 500));
    am->add(0, 0, s->f, thing, strlen(thing));
  }

  void init() {
    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    const Style *s2 = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c->addLayer(new MultiText(c, s));

    StyledMultiShape2D *p =
        c->addLayer(new StyledMultiShape2D(c, s, M_PI / 4, 0, 0));

    const Style *graphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    testBarChart(p, guiText);
    testButton(gui, guiText);
    testScrollBar(p, guiText);
    gui->drawRectangle(100, 100, 100, 100, grail::green);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestWidgets(), 1024, 600);
}
