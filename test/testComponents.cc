#include "grail/Grail.hh"
#include "opengl/primitivesV2/StyledMultiShape2D.hh"
#include "opengl/primitivesV2/MultiText.hh"
#include "opengl/GLWin.hh"
#include "opengl/primitivesV2/Colors.hh"
#include "Calendar.hh"

class TestComponents : public Grail {
public:
	TestComponents(uint32_t w, uint32_t h) : Grail(w,h) {}
	void init() {
		Canvas*c = currentTab()->getMainCanvas();
		Font* font = getDefaultFont();
		Style *s = new Style(font, 0,0,0, 0,0,0);
		StyledMultiShape2D* sms = new StyledMultiShape2D(s);
		MultiText* mt = new MultiText();
		c->addLayer(sms);
		c->addLayer(mt);
		Calendar cal;
	}
};
int main(int argc, char *argv[]) {
	return Grail::init(new TestComponents(1450, 1150), "out.cml");
}
