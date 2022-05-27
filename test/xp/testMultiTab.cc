#include <functional>

#include "opengl/GrailGUI.hh"
#include "opengl/NavigationBar.hh"

using namespace std;

class MultiTab : public NavigationBar {
 public:
  MultiTab(GLWin* w);
};

MultiTab::MultiTab(GLWin* w) : NavigationBar(w, 0, 0, 200, 130, 20, true) {
  ButtonWidget* prev = addButton(200, 30, "previous", "previous tab");
  prev->setAction(bind(&GLWin::prevTab, w));
  ButtonWidget* next = addButton(200, 30, "next", "next tab");
  next->setAction(bind(&GLWin::nextTab, w));
  ButtonWidget* add = addButton(200, 30, "+", "add tab");
  add->setAction(bind(&MultiTab::addNewTab, this));
}

void grailmain(int argc, char* argv[], GLWin* w) {
  new Tab(w);
  new MultiTab(w);
}
