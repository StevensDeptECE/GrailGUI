#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/ModifiableMultiShape.hh"

using namespace std;
using namespace grail;

class ReactingToInput : public Member {
 private:
  ModifiableMultiShape* m;
  float y = 0;

 public:
  ReactingToInput(Tab* t) : Member(t) {
    MainCanvas* c = t->getMainCanvas();
    ButtonWidget* b = new ButtonWidget(c, 200, 300, 100, 100, "add s", "add s");
    ButtonWidget* g = new ButtonWidget(c, 200, 500, 100, 100, "add l", "ad l");
    ButtonWidget* f =
        new ButtonWidget(c, 400, 300, 100, 100, "delete l", "delete l");
    ButtonWidget* d =
        new ButtonWidget(c, 600, 300, 100, 100, "delete s", "delete s");
    m = c->addLayer(new ModifiableMultiShape(c, c->getStyle(), t));
    d->setAction(bind(&ModifiableMultiShape::removeSolid, m, 0));
    b->setAction(bind(&ModifiableMultiShape::addfillRectangle, m));
    g->setAction(bind(&ModifiableMultiShape::drawRectangle, m));
    f->setAction(bind(&ModifiableMultiShape::removeline, m, 0));

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
