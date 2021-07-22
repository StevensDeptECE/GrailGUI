#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include "opengl/SparklineWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;


class TestWidgets : public GLWin {
 public:

  SparklineWidget *chart;
  double startTime;

  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}

  void testSparkline(StyledMultiShape2D *gui, MultiText *guiText) {

    vector <double> y2 = {20, 10, 20, 30, 35, 25, 15, 17, 30, 19, 11, 22, 29, 10, 24, 31, 34, 8, 9, 10};
    vector <double> xMax = {5};

    //graph values y2 from o to xMax
    chart = new SparklineWidget(gui, guiText, 75, 50, 200, 100, xMax, y2);
    chart->setAnimation(y2, 5, 100,grail::blue);
    //chart->chart(y2, x2, s2, 10000, 2, c2);

    chart->init();
  }

  void update(){
    
    double time = getTime();
    if (time > startTime + 0.01 ) {
      startTime = time;
      chart->update();
      setDirty();
    }

  }


  void init() {

    startTime = getTime();

    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    const Style *s2 = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c->getGuiText();
    
    testSparkline(gui, guiText);
    
  }
};

int main(int argc, char *argv[]) {
  
  return GLWin::init(new TestWidgets(), 1024, 600);

}
