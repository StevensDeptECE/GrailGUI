#include <string>
#include <vector>

#include "opengl/BarChartWidget.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/GraphWidget.hh"
#include "opengl/LineGraphWidget.hh"
#include "opengl/MultiText.hh"

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
    float x[] = {100, 10, 1000, 150, 10000};
    vector<float> y = {150, 350, 222, 100, 300};

    vector<string> labels = {"bar", "big bar", "other bar", "small bar",
                             "barrr"};
    BarChartWidget chart(gui, guiText, 50, 320, 400, 200);
    chart.chart(y, 0.25, labels, 50);
    chart.title("Title");
    chart.init();

#if 0
    std::string labels2[] = {"bar 1","bar 2","bar 3","bar 4", "bar 5"};
    BarChartWidget chart2(gui, guiText, 50, 320, 400, 200);
    chart2.chartLog(x, 5, 0.25, labels2, 10);
    chart2.title("Title Log graph");
    chart2.init();
#endif
  }

  void testLineGraphLinear(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {20, 40, 100, 200};
    vector<float> y = {100, 200, 50, 325};

    LineGraphWidget chart(gui, guiText, 50, 320, 400, 200);
    chart.chart(x, y, 50, 50, new LinearScale(), new LinearScale());
    chart.title("Title");
    chart.init();
  }
  void testLineGraphLog(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {20, 40, 100, 200};
    vector<float> y = {100, 1000, 10000, 10000000};

    LineGraphWidget chart(gui, guiText, 50, 320, 400, 200);
    chart.chart(x, y, 50, 50, new LinearScale(), new LogScale());
    chart.title("Title");
    chart.init();
  }

  void init() {
    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    const Style *s2 = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c->addLayer(new MultiText(c, s));

    // testBarChart(gui, guiText);
    // testButton(gui, guiText);
    testLineGraphLinear(gui, guiText);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestWidgets(), 600, 600);
}
