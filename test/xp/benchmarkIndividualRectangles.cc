#include "opengl/GrailGUI.hh"
#include "util/Benchmark.hh"
#include "opengl/Rectangle.hh"

using namespace std;
using namespace grail;
class BenchmarkIndividualRectangles : public Member {
 public:
  BenchmarkIndividualRectangles(Tab* tab) : Member(tab, -1, 0) {
    MainCanvas* c = tab->getMainCanvas();
    GLWin* w = tab->getParentWin();
    constexpr int n = 10000; // number of rectangles in test
    grail::utils::CBenchmark b("initialization");
    b.start();
    uint32_t width = 800;//tab->get
    float x = 0, y = 0;
    for (int i = 0; i < n; i++) {
      c->addLayer(new Rectangle(c, x, y, 2, 2, grail::red));
      x += 3;
      if (x > width) {
        x = 0, y += 3;
      }
    }
    b.end();
    b.display();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test Individual Rectangle Render");
  new BenchmarkIndividualRectangles(defaultTab);
}
