#include <functional>

#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;

class MultiTab : public Member {
 private:
  ButtonWidget prev, next, add;

 public:
  MultiTab(Tab* tab)
      : Member(tab),
        prev(c, 0, 0, 50, 30, "previous", "previous tab"),
        next(c, 60, 0, 50, 30, "next", "next tab"),
        add(c, 120, 0, 30, 30, "+", "add tab") {
    GLWin* w = tab->getParentWin();
    prev.setAction(bind(&GLWin::prevTab, w));
    next.setAction(bind(&GLWin::nextTab, w));
    add.setAction(bind(&GLWin::addTab, w));
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  defaultTab->addMember(new MultiTab(defaultTab));
}
