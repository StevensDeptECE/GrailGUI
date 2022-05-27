#include "fmt/core.h"
#include "opengl/GrailGUI.hh"
#include "opengl/NavigationBar.hh"
#include "util/Ex.hh"

using namespace std;
using namespace grail;

class Square : public Member {
 public:
  Square(GLWin* w) : Member(w, 0, 0) {
    StyledMultiShape2D* gui = tab->getMainCanvas()->getGui();
    gui->fillRectangle(500, 600, 200, 200, red);
  }
};

class Circle : public Member {
 public:
  Circle(GLWin* w) : Member(w, 0, 0) {
    StyledMultiShape2D* gui = tab->getMainCanvas()->getGui();
    gui->fillCircle(600, 700, 100, 5, purple);
  }
};

class MultiTab : public NavigationBar {
 public:
  MultiTab(GLWin* w);
};

MultiTab::MultiTab(GLWin* w) : NavigationBar(w, 0, 0, 250, 200, 20, true) {
  ButtonWidget* square = addButton(250, 50, "Square", "tab 1");
  square->setAction([w]() { w->switchTab(0); });

  ButtonWidget* circle = addButton(250, 50, "Circle", "tab 2");
  circle->setAction([w]() { w->switchTab(1); });

  ButtonWidget* next = addButton(250, 50, "Next", "next tab");
  next->setAction([w]() { w->nextTab(); });
}

void grailmain(int argc, char* argv[], GLWin* w) {
  w->setTitle("Test Tabs");
  new Square(w);
  MultiTab* m = new MultiTab(w);
  new Circle(w);
}