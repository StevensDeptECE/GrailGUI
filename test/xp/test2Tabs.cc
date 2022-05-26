#include <iostream>

#include "fmt/core.h"
#include "opengl/GrailGUI.hh"
#include "opengl/NavigationBar.hh"
#include "util/Ex.hh"

using namespace std;
using namespace grail;

class Member1 : public Member {
 public:
  Member1(Tab* tab) : Member(tab, 0, 0) {
    MainCanvas* c = tab->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    gui->fillRectangle(500, 600, 200, 200, red);
  }
};

class Member2 : public Member {
 public:
  Member2(Tab* tab) : Member(tab, 0, 0) {
    MainCanvas* c = tab->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    gui->drawCircle(450, 50, 45, 5, purple);
  }
};

class MultiTab : public NavigationBar {
 public:
  MultiTab(GLWin* w);
};

MultiTab::MultiTab(GLWin* w) : NavigationBar(w, 0, 0, 200, 130, 20, true) {
  ButtonWidget* hi = addButton(200, 30, 0, "hi", "print hi");
  auto action = []() { fmt::print("hi\n"); };
  hi->setAction(action);

  ButtonWidget* next = addButton(200, 30, 50, "next", "next tab");
  auto action2 = [w]() { w->nextTab(); };
  next->setAction(action2);
}

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test Tabs");
  new Member1(defaultTab);
  MultiTab* m = new MultiTab(w);
  Tab* t2 = w->addTab();
  new Member2(t2);
}