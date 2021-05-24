#include "opengl/GrailGUI.hh"
using namespace std;

class SimpleDemo : public GLWin {
 public:
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    c->addButton("test", 0,0, 600, 100);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new SimpleDemo());
}
