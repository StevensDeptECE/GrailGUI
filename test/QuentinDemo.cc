#include "opengl/GrailGUI.hh"
#include"opengl/ButtonWidget.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
public:
	TestMultiShape() :
		GLWin(0x000000, 0xCCCCCC, "Test StyledMultiShape") {}
	void init() {
		MainCanvas* c = currentTab()->getMainCanvas();
		StyledMultiShape2D* gui = c->getGui();

    

		const float boxSize = 75;
		const float drawSize = (boxSize/4)*5;

    float xy2[] = {boxSize+drawSize/2, drawSize/2, drawSize/2, boxSize+drawSize/2, 7*boxSize+drawSize/2, 5*boxSize+drawSize/2, 8*boxSize+drawSize/2, 4*boxSize+drawSize/2};
		gui->fillPolygon(xy2, 4, blue);
    
    
    gui->fillRectangle(boxSize, boxSize, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize, boxSize, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize, boxSize*2, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*2, boxSize*1.5, drawSize/5, drawSize*.4, black);
    gui->fillRectangle(boxSize*1.5, boxSize*1.5, drawSize*.4, drawSize/5, black);

    gui->fillRectangle(boxSize*2.5, boxSize*1.75, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*2.5, boxSize*1.75, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*2.5, boxSize*2.25, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*3.5, boxSize*1.75, drawSize/5, drawSize*.4, black);
    gui->fillRectangle(boxSize*2.75, boxSize*2.5, drawSize*.4, drawSize/5, black);
    gui->fillRectangle(boxSize*3.25, boxSize*2.75, drawSize*.4, drawSize/5, black);
    

    gui->fillRectangle(boxSize*4, boxSize*2.5, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*4, boxSize*2.5, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*5, boxSize*2.5, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*4, boxSize*3, drawSize, drawSize/5, black);

    gui->fillRectangle(boxSize*5.5, boxSize*3.25, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*5.5, boxSize*4.25, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*6, boxSize*3.25, drawSize/5, drawSize, black);
   
    gui->fillRectangle(boxSize*7, boxSize*4, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*7, boxSize*5, drawSize, drawSize/5, black);

    const char buttonName[] = "beegus";
    MultiText* guiText = c->getGuiText();
    //ButtonWidget b(gui, guiText, "hello", 0, 0, 100, 50);
	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new TestMultiShape());
}
