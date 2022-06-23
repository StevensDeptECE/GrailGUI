#include <functional>

#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/NavigationBar.hh"

using namespace std;

class MultiTab : public Member {
 private:
  ButtonWidget prev, next, add;
  GLWin* parentWin;

 public:
  void addTab();

  MultiTab(Tab* tab);
};

void MultiTab::addTab() {
  Tab* t = parentWin->addTab();
  t->addMember(this);
  t->setUpdate();
}

MultiTab::MultiTab(Tab* tab)
    : Member(tab),
      prev(c, 0, 0, 200, 30, "previous", "previous tab"),
      next(c, 0, 50, 200, 30, "next", "next tab"),
      add(c, 0, 100, 200, 30, "+", "add tab") {
  parentWin = tab->getParentWin();
  prev.setAction(bind(&GLWin::prevTab, parentWin));
  next.setAction(bind(&GLWin::nextTab, parentWin));
  add.setAction(bind(&MultiTab::addTab, this));
}

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  new MultiTab(defaultTab);
  new NavigationBar(defaultTab, 0, 500, 600, 50, 2);
}
