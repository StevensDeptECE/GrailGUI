#include "opengl/ButtonWidget.hh"
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

    uint32_t rectangle1 = m->addfillRectangle(100, 100, 100, 100, red);
    uint32_t rectangle2 = m->addfillRectangle(300, 100, 100, 100, green);
    uint32_t rectangle3 = m->addfillRectangle(500, 100, 100, 100, blue);
    uint32_t rectangle4 = m->addfillRectangle(700, 100, 100, 100, yellow);
    uint32_t rectangle5 = m->addfillRectangle(900, 100, 100, 100, black);
    m->removeSolid(rectangle1);

    //  ideally put action on button to delete so you can see it in real time
  }

  /*
    void addNewRectangle() {
      handle = m->fillRectangle(10, y, 200, 200, red);
      y += 50;
      m->update();
    }
    void removeLastRectangle() {
      // remove the last one on the list in ModifiableMultiShape?
    }*/
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Simple add and remove draw elements");
  w->setSize(1024, 800);
  new ReactingToInput(t);
}