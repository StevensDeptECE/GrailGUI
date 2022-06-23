#include "opengl/GrailGUI.hh"
#include "opengl/ModifiableMultiShape.cc"

using namespace std;
using namespace grail;

class ReactingToInput : public Member {
 private:
  ModifiableMultiShape* m;
  float y = 0;

 public:
  ReactingToInput(Tab* t) : Member(t) {
    MainCanvas* c = t->getMainCanvas();
    m = c->addLayer(new ModifiableMultiShape(c, c->getStyle(), t));

//====================Solid Delete Testing=================//
#if 1
    uint32_t roundrect1 =
        m->addfillRoundRect(300, 100, 100, 100, 20, 20, green);
    uint32_t rectangle1 = m->addfillRectangle(100, 100, 100, 100, red);
    uint32_t rectangle2 = m->addfillRectangle(500, 100, 100, 100, blue);
    uint32_t triangle1 = m->addfillTriangle(550, 300, 650, 250, 700, 300, blue);
    uint32_t rectangle3 = m->addfillRectangle(700, 100, 100, 100, yellow);
    uint32_t rectangle4 = m->addfillRectangle(900, 100, 100, 100, black);
    uint32_t circle1 = m->addfillCircle(200, 600, 30, 30, green);
    uint32_t ellipse1 = m->addfillEllipse(300, 600, 20, 20, 20, darkgreen);
    uint32_t polygon1 = m->addfillPolygon(900, 500, 100, 100, 6, black);
    m->removeSolid(rectangle4);
    m->removeSolid(rectangle1);
    m->removeSolid(triangle1);
    m->removeSolid(polygon1);
    m->removeSolid(roundrect1);
    m->removeSolid(circle1);
    m->removeSolid(ellipse1);
#endif
//=================Line Delete Testing=====================//
#if 0
    uint32_t rectangle = m->adddrawRectangle(100, 100, 200, 200, red);
    m->removeLine(rectangle);

#endif
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Simple add and remove draw elements");
  w->setSize(1024, 800);
  new ReactingToInput(t);
}