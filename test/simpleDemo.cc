#include "opengl/GrailGUI.hh"
using namespace std;

class SimpleDemo : public GLWin {
 public:
  void init() {
    Style* s = getDefaultStyle();
    Canvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(s));
    m->fillRectangle(0, 0, 512, 512, grail::red);

    currentTab()->addButton(s, "testing", 500, 200, 100, 50);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new SimpleDemo());
}
