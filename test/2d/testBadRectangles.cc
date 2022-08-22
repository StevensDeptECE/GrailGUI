#include "openGL/GrailGUI.hh"
#include "opengl/Rect.hh"

class BadRectangles : public Member {
 public:
  BadRectangles(Tab* tab) : Member(tab, 0, 0) {
    MainCanvas* c = tab->getMainCanvas();
    for (int i = 0; i < 10000; i++)
      c->addLayer(new Rect(c, 100, 100, 200, 200, 1.0, 0, 0));
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test Bad Rectangles");
  new BadRectangles(defaultTab);
}