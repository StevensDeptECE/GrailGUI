#define _USE_MATH_DEFINES

#include <cmath>
#include <string>
#include <vector>

#include "opengl/AngledMultiText.hh"
#include "opengl/BarChartWidget.hh"
#include "opengl/BoxChartWidget.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/CandlestickChartWidget.hh"
#include "opengl/GapMinderWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/LineGraphWidget.hh"
#include "opengl/ScrollbarWidget.hh"
#include "opengl/SparklineWidget.hh"
#include "opengl/util/Transformation.hh"

using namespace std;
using namespace grail;

class TestWidgets : public GLWin {
 private:
  ScrollbarWidget *scrollbar;

 public:
  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}

  static void scrollUp(GLWin *w) {
    TestWidgets *tw = (TestWidgets *)w;

    tw->scrollbar->scroll(-5);
    tw->setDirty();
  }
  static void scrollDown(GLWin *w) {
    TestWidgets *tw = (TestWidgets *)w;

    tw->scrollbar->scroll(5);
    tw->setDirty();
  }

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

  void testCandlestick(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> y = {
        153.25, 154.16, 152.99, 153.68, 153.34, 153.73, 152.15, 152.73, 152.66,
        153.58, 151.46, 153.57, 153.71, 153.9,  152.17, 153.26, 153.46, 154.53,
        153.36, 153.92, 153.08, 153.31, 151.91, 152.93, 153.63, 154.88, 153.5,
        154.83, 154.41, 154.93, 154.01, 154.74, 155.25, 156.17, 154.93, 155.28,
        155.5,  157.43, 154.54, 155.34, 155.41, 156.42, 155.19, 155.99, 155.79,
        156.81, 155.17, 155.44, 156.17, 156.72, 155.85, 156.43, 157.33, 159.43,
        157.09, 159.31, 159.21, 160.29, 158.66, 159.9,  159.85, 160.13, 159.03,
        159.19, 156.19, 156.52, 154.46, 155.42, 156.05, 157.18, 155.4,  155.69,
        156.33, 157.13, 154.94, 155.61, 155.73, 156.86, 155.64, 156.54, 156.35,
        156.99, 154.71, 155.85, 156.67, 157.26, 156.22, 156.85, 158.72, 163.01,
        158.13, 162.47, 163.3,  167.47, 163.13, 166.12, 167.3,  169.04, 166.34,
        168.43, 169.26, 169.33, 165.02, 166.29, 167.04, 167.9,  164.69, 167.51,
        173.38, 173.64, 170.51, 171.88, 171.75, 174.36, 171.1,  173.63, 173.29,
        174.51, 173.29, 174.18, 174.03, 175.61, 173.71, 175.61, 174.48, 175.46,
        172.52, 175.25, 175.11, 175.38, 174.27, 174.67};

    CandlestickChartWidget chart(gui, guiText, 50, 50, 400, 200, y);
    chart.setMinMax(150, 180);
    chart.chart(y, 5, 4);
    chart.setTitle("Title");
    chart.init();
  }

  void testBoxChart(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> y = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                       170, 100, 50,  20,  150, 200, 330, 200, 270,
                       180, 300, 49,  247, 325, 114, 89};
    vector<string> labels2 = {"box 1", "box 2", "box 3", "box 4", "box 5"};
    BoxChartkWidget chart(gui, guiText, 50, 320, 400, 200, y);
    chart.chart(y, 50, 5, labels2);
    chart.setTitle("Title");
    chart.init();
  }

  void testGapMinder(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {100, 220, 50, 150, 300, 290, 230};
    vector<float> y = {150, 350, 222, 100, 290, 60, 200};
    vector<float> s = {3, 2, 6, 5, 4, 10, 15};
    vector<glm::vec4> c = {grail::red,  grail::blue,   grail::green,
                           grail::cyan, grail::purple, grail::darkblue};

    GapMinderWidget chart(gui, guiText, 550, 320, 400, 200, x, y);
    chart.chart(y, x, s, 50, c);

    chart.setTitle("Title");
    chart.init();
  }

  void testSparkline(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {0, 50, 100, 150, 200, 250, 300};
    vector<float> y = {150, 350, 222, 100, 290, 60, 200};
    vector<float> y2 = {20, 74, 42, 80, 20, 60, 37};

    SparklineWidget chart(gui, guiText, 550, 50, 150, 50, x, y);
    chart.chart(y, x, grail::blue);
    chart.init();

    SparklineWidget chart2(gui, guiText, 550, 110, 150, 50, x, y2);
    chart2.chart(y2, x, grail::red);
    chart2.init();
  }

#if 0
  void testLineGraphLinear(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {20, 40, 100, 200};
    vector<float> y = {100, 200, 50, 325};

    LineGraphWidget chart(gui, guiText, 50, 320, 400, 200);
    chart.chart(x, y, 50, 50, new LinearScale(), new LinearScale());
    chart.title("Title");
    chart.init();
  }
#endif

#if 0
  void testLineGraphLog(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {20, 40, 100, 200};
    vector<float> y = {100, 1000, 10000, 10000000};

    LineGraphWidget chart(gui, guiText, 50, 320, 400, 200);
    chart.chart(x, y, 50, 50, new LinearScale(), new LogScale());
    chart.title("Title");
    chart.init();
  }
#endif

  void testScrollBar(StyledMultiShape2D *gui, MultiText *guiText) {
    const uint32_t scrollBarWidth = 50;
    //    ScrollbarWidget scrollBar(gui, guiText, getWidth() - scrollBarWidth,
    //    0, scrollBarWidth, getHeight()); scrollBar.init();
    MainCanvas *c = currentTab()->getMainCanvas();
    const Style *s = new Style(nullptr, grail::black, grail::blue);
    scrollbar = c->addLayer(new ScrollbarWidget(c, s, width - scrollBarWidth, 0,
                                                scrollBarWidth, getHeight()));
  }

  void testAngleText(StyledMultiShape2D *gui, MultiText *guiText, Canvas *c,
                     const Style *s) {
    const char thing[] = "hello world";
    // guiText->add(0, 50, s->f, thing, strlen(thing));
    AngledMultiText *am =
        c->addLayer(new AngledMultiText(c, s, M_PI / 2, 20, 500));
    am->add(200, 600, s->f, thing, strlen(thing));
  }

  void init() {
    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    const Style *s2 = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();
    StyledMultiShape2D *test = c->addLayer(new StyledMultiShape2D(c, s2));

    MultiText *guiText = c->addLayer(new MultiText(c, s));

    StyledMultiShape2D *p =
        c->addLayer(new StyledMultiShape2D(c, s, -M_PI / 4, 0, 0));

    const Style *graphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    // testBarChart(gui, guiText);
    testCandlestick(gui, guiText);
    testBoxChart(gui, guiText);
    testGapMinder(gui, guiText);
    testSparkline(gui, guiText);
    // testButton(gui, guiText);
    // testLineGraphLinear(gui, guiText);
    // testLineGraphLog(gui, guiText);
    // testLinearAxesWidget(gui, guiText, graphStyle);
    // testAngleText(gui, guiText, c, s);

    testScrollBar(gui, guiText);

    setAction(1002, scrollUp);
    setAction(1003, scrollDown);

    setEvent(401, 1002);
    setEvent(399, 1003);
  }

  void update() { scrollbar->update(); }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestWidgets(), 1024, 600);
}
