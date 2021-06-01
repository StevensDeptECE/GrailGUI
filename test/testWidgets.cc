#include "opengl/GrailGUI.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GraphWidget.hh"
#include "opengl/MultiText.hh"
using namespace std;
using namespace grail;

class TestWidgets : public GLWin {
 public:
 TestWidgets() :
 
		GLWin(0x000000, 0xCCCCCC, "Window") {}
  void init() {
    
    const Style* s = new Style("TIMES", 24, 1,
          0,0,0,
          0,1,0);

    const Style* s2 = new Style("TIMES", 24, 1,
          0,1,0,0,
          0,0,0,0);          

    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    //MultiText* guiText = c->getGuiText();
    MultiText* guiText = c->addLayer(new MultiText(c,s));

    const float boxSize = 100;
		const float drawSize = (boxSize/4)*5;

    gui->fillRectangle(boxSize, boxSize, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize, boxSize, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize, boxSize*2, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*2, boxSize*1.5, drawSize/5, drawSize*.4, black);
    gui->fillRectangle(boxSize*1.5, boxSize*1.5, drawSize*.4, drawSize/5, black);

    
    float x[] = {100, 200, 300};
    float y[] = {50, 350, 222};

    //c->addButton("Hello",100,100,100,100);
    ButtonWidget b(gui, guiText, "hello", 0, 0, 100, 50);
    b.init();

  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestWidgets(), 512, 512);
}
