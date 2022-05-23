#include "opengl/GrailGUI.hh"
using namespace std;

class SimpleDemo : public GLWin {
 public:
  void init() {
    //    const Style* s = getDefaultStyle();
    const Style* s = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    c->addButton("test", 0, 0, 600, 100);
    MultiText* guiText = new MultiText(c, s, 1024);  // c->getGuiText();
    const Font* f = getDefaultFont();
    guiText->add(10.0f, 50.0f, f, "testing", 7);
    // c->addLayer(mt);
    //    string menuText[] = {"New", "Open...", "Save", "Save As...", "Quit"};
    //    c->addMenu(menuText, sizeof(menuText)/sizeof(string), 512,0);
    // StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, s));
    // gui->fillRectangle(0, 0, 512, 512, grail::red);

    //    currentTab()->addButton(s, "testing", 500, 200, 100, 50);
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new SimpleDemo()); }
