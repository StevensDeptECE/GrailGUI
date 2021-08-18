#include <functional>

#include "opengl/Animated.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;

class NetworkedButton : public Animated {
  ButtonWidget button;

 public:
  NetworkedButton(Tab* tab)
      : Animated(tab), button(c, 0, 0, 100, 100, "", "click!") {
    button.setAction(
        bind(&GLWin::goToLink, tab->getParentWin(), "127.0.0.1", 8060, 0));
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  defaultTab->addAnimated(new NetworkedButton(defaultTab));
}