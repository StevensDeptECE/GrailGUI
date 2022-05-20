#include "grail/Grail.hh"
#include "opengl/primitivesV2/MultiText.hh"
#include "opengl/primitivesV2/StyledMultiShape2D.hh"
#include "opengl/GLWinFonts.hh"
using namespace std;

class TestText : public Grail {
public:
	void init() {
		Canvas *c = currentTab()->getMainCanvas();
		Style *s = c->getStyle();
		s->setShaderIndex(Grail::TEXT_SHADER);
    StyledMultiShape2D* ms = c->addLayer(new StyledMultiShape2D(s));
//    ms->fillRectangle(10,10, width-20,height-20, grail::black);
//    ms->fillRectangle(400,100, 100,50, grail::yellow);
    cout << width << "," << height << '\n';
		MultiText *m = c->addLayer(new MultiText(s, 100000 * (6 + 3 + 6 + 4)));
    float y = 100;
    const Font* f = FontFace::get("TIMES", 20, 0);
    m->addChar(100, 100, f, (unsigned char)'D');
    m->addChar(300, 100, f, (unsigned char)'K');
	}
};

int main(int argc, char *argv[]) {
	return Grail::init(new TestText(), "out.cml");
}
