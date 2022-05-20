#include "opengl/GrailGUI.hh"
#include "opengl/PopupMenu.hh"

using namespace std;
using namespace grail;

class TestPopupMenu : public GLWin {
 private:
   PopupMenu* p;
 public:
  TestPopupMenu() : GLWin(0x000000, 0xCCCCCC, "Test Popup Menu") {}

  void init() {
    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D* m = c->getGui();
    MultiText* t = c->getGuiText();

    const Font* f = getDefaultFont();
    const string menuItems[] = {"Click me!", "test", "test2", "Quit"};
    p = new PopupMenu(m, t, f, 10, 10, 300, 500, menuItems, 4);
    p->init();
  }
  void cleanup() {
    delete p;
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestPopupMenu(), 1000, 1000);
}
