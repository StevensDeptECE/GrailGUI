#include "openGL/GrailGUI.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/Rect.hh"

using namespace grail;
#if 1
class GoodRectangles : public Member {
 public:
  GoodRectangles(Tab* tab) : Member(tab, 0, -1) {
    MainCanvas* c = tab->getMainCanvas();
    const Style* s = c->getStyle();

#if 1
    StyledMultiShape2D* rectangleLayer =
        c->addLayer(new StyledMultiShape2D(c, s));
    for (int i = 0; i < 100000; i++) {
      rectangleLayer->fillRectangle(100, 100, 200, 200, red);
    }
#endif
  }
};
#endif

#if 1
class BadRectangles : public Member {
 public:
  BadRectangles(Tab* tab) : Member(tab, 0, -1) {
    MainCanvas* c = tab->getMainCanvas();
    for (int i = 0; i < 100000; i++)
      c->addLayer(new Rect(c, 100, 100, 200, 200, 1.0, 0, 0));
  }
};
#endif

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test Bad Rectangles");
  new GoodRectangles(defaultTab);
  // new BadRectangles(defaultTab);
}