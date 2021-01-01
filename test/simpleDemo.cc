#include "opengl/GrailGUI.hh"
using namespace std;

class SimpleDemo : public GLWin {
 public:
  void init() {
    const Style* s = getDefaultStyle();
    MainCanvas* c = currentTab()->getMainCanvas();
    c->addButton("test", 0,0, 100, 100);
    string menuText[] = {"New", "Open...", "Save", "Save As...", "Quit"};
    c->addMenu(menuText, sizeof(menuText)/sizeof(string), 512,0);
    StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, s));
    m->fillRectangle(0, 0, 512, 512, grail::red);

		//    currentTab()->addButton(s, "testing", 500, 200, 100, 50);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new SimpleDemo());
}
