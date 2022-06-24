#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/ModifiableMultiShape.cc"

using namespace std;
using namespace grail;

class ReactingToInput : public Member {
 private:
  ModifiableMultiShape* m;
  float x = 0;
  float y = 0;
  float w = 200;
  float h = 200;
  const glm::vec4& c = red;

 public:
  bool recton = true;
  bool roundrecton = true;
  bool triangleon = true;
  uint32_t rect;
  uint32_t roundRect;
  uint32_t triangle;
  ReactingToInput(Tab* t) : Member(t) {
    MainCanvas* c = t->getMainCanvas();

    m = c->addLayer(new ModifiableMultiShape(c, c->getStyle(), t));
    rect = m->addfillRectangle(50, 100, 100, 100, red);
    roundRect = m->addfillRoundRect(300, 200, 100, 100, 50, 50, green);
    triangle = m->addfillTriangle(550, 200, 500, 250, 600, 250, blue);
    ButtonWidget* rectButton =
        new ButtonWidget(c, 50, 500, 200, 200, "Rect", "remove");
    ButtonWidget* roundRectButton =
        new ButtonWidget(c, 300, 500, 200, 200, "RoundRect", "remove");
    ButtonWidget* triButton =
        new ButtonWidget(c, 550, 500, 200, 200, "Triangle", "remove");

    rectButton->setAction(bind(&ReactingToInput::toggleRect, this, rect));
    roundRectButton->setAction(
        bind(&ReactingToInput::toggleRoundRect, this, roundRect));
    triButton->setAction(
        bind(&ReactingToInput::toggleTriangle, this, triangle));
    //  ideally put action on button to delete so you can see it in real time
  }

  void toggleRect(uint32_t index) {
    if (recton == true) {
      m->removeSolid(index);
      recton = false;
    } else {
      rect = m->addfillRectangle(50, 100, 100, 100, red);
      recton = true;
    }
    m->init();
    tab->getParentWin()->setUpdate();
  }

  void toggleRoundRect(uint32_t index) {
    if (roundrecton == true) {
      m->removeSolid(index);
      roundrecton = false;
    } else {
      roundRect = m->addfillRoundRect(300, 200, 100, 100, 50, 50, green);
      roundrecton = true;
    }
    m->init();
    tab->getParentWin()->setUpdate();
  }
  void toggleTriangle(uint32_t index) {
    if (triangleon == true) {
      m->removeSolid(index);
      triangleon = false;
    } else {
      triangle = m->addfillTriangle(550, 200, 500, 250, 600, 250, blue);
      triangleon = true;
    }
    m->init();
    tab->getParentWin()->setUpdate();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Simple add and remove draw elements");
  w->setSize(1024, 800);
  new ReactingToInput(t);
}
