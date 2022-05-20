#include "opengl/GrailGUI.hh"
using namespace std;

class TestGrid : public GLWin {
public:
	void init() {
		const Style* s = getDefaultStyle();
		Canvas* c = currentTab()->addCanvas(s, 0, 0, width, height);
		StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, s));
		m->fillGrid(200,200, 250, 250, 10, 5, grail::blue, grail::lightblue);
	}
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestGrid());
}
