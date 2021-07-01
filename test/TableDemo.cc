#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "opengl/TableWidget.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
 public:
  TestMultiShape() : GLWin(0x000000, 0xCCCCCC, "Test StyledMultiShape") {}

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    MultiText* guiText = c->getGuiText();

    int x = 100;
    int y = 100;
    int w = 100;
    int h = 100;
    int tH = 4;
    int tW = 5;

    TableWidget a(gui,guiText,x,y,w,h,tH,tW);
    a.init();
/*
    gui->drawRectangle(x,y,w,h,black);

    if(tH > 0){
      for(int i = 1; i < tH; i++){
        gui->drawLine(x,(i*(y/tH))+h,x+w,(i*(y/tH))+h,black);
      }
    }

    if(tW > 0){
      for(int i = 1; i < tW; i++){ 
        gui->drawLine(i*(x/tW)+w,y,i*(x/tW)+w,y+h,black);
      }
    }

*/
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestMultiShape());
}
