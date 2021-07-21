#include <cmath>
#include <numbers>
#include <string>
#include <vector>

#include "opengl/ButtonWidget.hh"
#include "opengl/CandlestickChartWidget.hh"
#include "opengl/GapMinderWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/PositionTool.hh"
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

  void scrollUp() {
    scrollbar->scroll(-5);
    setDirty();
  }
  void scrollDown() {
    scrollbar->scroll(5);
    setDirty();
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

  void testGapMinder(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> x = {100, 220, 50, 150, 300, 290, 230};
    vector<float> y = {150, 350, 222, 100, 290, 60, 200};
    vector<float> s = {3, 2, 6, 5, 4, 10, 15};
    vector<glm::vec4> c = {grail::red,  grail::blue,   grail::green,
                           grail::cyan, grail::purple, grail::darkblue};

    GapMinderWidget chart(gui, guiText, 550, 320, 400, 200, x, y);
    chart.chart(y, x, s, 50, 50, c);

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
    MultiText *mt = c->addLayer(new MultiText(c, s, numbers::pi / 2, 100, 100));
    mt->add(0, 0, s->f, thing, strlen(thing));
  }

  void testPositionTool(StyledMultiShape2D *gui, MultiText *guiText) {
    vector<float> X = {334, 56};
    vector<float> Y = {234, 52};
    vector<float> R = {45, 34};

    PositionTool test(gui, guiText, 50, 320, 400, 200, X, Y, R);
    test.addNewCord(123, 53, 25);
    test.update(123, 345, 1);
    test.init();
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
        c->addLayer(new StyledMultiShape2D(c, s, -numbers::pi / 4, 0, 0));

    const Style *graphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    // testCandlestick(gui, guiText);
    testGapMinder(gui, guiText);
    testSparkline(gui, guiText);
    // testButton(gui, guiText);
    testAngleText(gui, guiText, c, s);

    testScrollBar(gui, guiText);

    bindEvent(Inputs::WHEELUP, &TestWidgets::scrollUp, this);
    bindEvent(Inputs::WHEELDOWN, &TestWidgets::scrollDown, this);
    testPositionTool(gui, guiText);
  }

  void update() { scrollbar->update(); }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestWidgets(), 1024, 600);
}
