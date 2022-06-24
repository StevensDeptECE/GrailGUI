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
    m->removeSolid(rect);
    rect = m->addfillRectangle(50, 100, 100, 100, red);
    m->removeSolid(rect);
    rect = m->addfillRectangle(50, 100, 100, 100, red);

    ButtonWidget* rectButton =
        new ButtonWidget(c, 50, 500, 200, 200, "Rect", "remove");
    ButtonWidget* roundRectButton =
        new ButtonWidget(c, 300, 500, 200, 200, "RoundRect", "remove");
    ButtonWidget* triButton =
        new ButtonWidget(c, 550, 500, 200, 200, "Triangle", "remove");

    rectButton->setAction(bind(&ReactingToInput::toggleRect, this));
    roundRectButton->setAction(bind(&ReactingToInput::toggleRoundRect, this));
    triButton->setAction(bind(&ReactingToInput::toggleTriangle, this));
    //  ideally put action on button to delete so you can see it in real time
  }

  void toggleRect() {
    if (recton == true) {
      m->removeSolid(rect);
      recton = false;
    } else {
      rect = m->addfillRectangle(50, 100, 100, 100, red);
      recton = true;
    }
    m->updatePoints();
    tab->getParentWin()->setUpdate();
  }

  void toggleRoundRect() {
    if (roundrecton == true) {
      m->removeSolid(roundRect);
      roundrecton = false;
    } else {
      roundRect = m->addfillRoundRect(300, 200, 100, 100, 50, 50, green);
      roundrecton = true;
    }
    m->updatePoints();
    tab->getParentWin()->setUpdate();
  }
  void toggleTriangle() {
    if (triangleon == true) {
      m->removeSolid(triangle);
      triangleon = false;
    } else {
      triangle = m->addfillTriangle(550, 200, 500, 250, 600, 250, blue);
      triangleon = true;
    }
    m->updatePoints();
    tab->getParentWin()->setUpdate();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Simple add and remove draw elements");
  w->setSize(1024, 800);
  new ReactingToInput(t);
}
