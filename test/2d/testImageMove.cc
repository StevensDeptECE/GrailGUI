#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"

using namespace std;
using namespace grail;

// void f(int x, int y) {}
class ReactingToInput : public Member {
 private:
  float y = 0;
  bool hasclicked = false;
  bool notpressed;
  void (*p)();  // p points to void functions with empty ()
  float xpos, ypos, width, height;
  Image* piece;

  void GetPosition(GLWin* w) {
    xpos = w->mouseX;
    ypos = w->mouseY;
  }

 public:
  ReactingToInput(Tab* t, GLWin* w) : Member(t) {
    MainCanvas* c = t->getMainCanvas();

    GetPosition(w);
    xpos = 50;
    ypos = 500;
    width = 200;
    height = 200;

    piece = c->addLayer(
        new Image(c, 100, 100, 200, 200, "ChessTextures/brook.webp"));
    // c->removeLayer(PieceButton);

    /*int foo = 1;

    auto fn1 = [foo]() { cout << foo; };
    auto fn2 = [=]() { cout << foo; };  // captures all variables by value
    auto fn3 = [&]() { cout << foo; };  // captures all variables by reference*/

    // auto fn1 = [](GLWin* w) { return w; };
    // t->bindEvent(Tab::MOUSE0_PRESS, &ReactingToInput::press, this);

    t->bindEvent(Tab::MOUSE0_PRESS, [w, this]() { press(w); });
    t->bindEvent(Tab::MOUSE0_RELEASE, &ReactingToInput::release, this);
  }

  void press(GLWin* w) {
    // turn click into which box you are in
    //  (y - offset) / boxsize = box you are in
    if (hasclicked == false) {
      cout << w->mouseX << " " << w->mouseY << " "
           << "Position Variables"
           << "\n";
      hasclicked = true;
    } else {
      piece = addNewImage(w->mouseX, w->mouseY);
      hasclicked = false;
      tab->getParentWin()->setUpdate();
    }
  }
  void release() {
    if (hasclicked) {
      cout << "RELEASE WORKS!!!!"
           << "\n";
      cout << "Click to make reappear"
           << "\n";
      c->removeLayer(piece);
      tab->getParentWin()->setUpdate();
    } else {
      return;
    }
  }

  void drag() {
    // When mouse moves:
    if (notpressed) return;
  }

  Image* addNewImage(double xpos, double ypos) {
    Image* m = c->addLayer(
        new Image(c, xpos, ypos, 200, 200, "ChessTextures/brook.webp"));
    m->init();
    return m;
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Simple add and remove draw elements");
  w->setSize(1024, 800);
  new ReactingToInput(t, w);
}