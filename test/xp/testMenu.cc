#include "opengl/DropDownMenu.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class Square : public Member {
 public:
  Square(GLWin* w) : Member(w, "Square", 0, 0) {
    StyledMultiShape2D* gui = tab->getMainCanvas()->getGui();
    gui->fillRectangle(500, 600, 200, 200, red);
  }
};

class Circle : public Member {
 public:
  Circle(GLWin* w) : Member(w, "Circle", 0, 0) {
    StyledMultiShape2D* gui = tab->getMainCanvas()->getGui();
    gui->fillCircle(600, 700, 100, 5, purple);
  }
};

void grailmain(int argc, char* argv[], GLWin* w) {
  w->setTitle("Test Tabs");
  Style* s = new Style(FontFace::get("TIMES", 28, FontFace::BOLD), grail::black,
                       grail::lightblue, 2);
  new DropDownMenu(w, 0, 0, "Tabs", 5, 0, 0, s);
  new Square(w);
  new Circle(w);
}