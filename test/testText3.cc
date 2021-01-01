#include "opengl/GrailGUI.hh"
using namespace std;

class TestText3 : public GLWin {
public:

  void sampleText(MultiText* m, const Font* f, float y) {
    m->add(20, y, f, 123);
    m->add(150, y, f, "Dov", 3);
    m->add(250, y, f, "Kruger", 6);
    m->add(450, y, f, 123456.789);
    m->addHex8(750,y, f, 0x0034ABCD);
  }

  void sampleText2(MultiText* m, const Font* f, float y) {
    static const char s[] = "!\"#$%&'()*+0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    m->add(20, y, f, s, sizeof(s)-1);
  }

  float testOneFontFace(MultiText* m, const char fontFamily[], float yStart) {
    const int sizes[] = {40, 30, 20, 10};
    for (auto size : sizes) {
      const Font* font = FontFace::get(fontFamily, size, 0);
      sampleText2(m, font, yStart);
      yStart += size + 4;
    }
    return yStart+50;
  }

	void init() {
		Canvas *c = currentTab()->getMainCanvas();
		const Font* f = FontFace::get("TIMES", 16, FontFace::BOLD);

		Style* s = new Style(f, 0,0,0, 0,.2,0.9, GLWin::TEXT_SHADER);

		//    const Font* f = getDefaultFont();
		//		s->setShaderIndex(GLWin::TEXT_SHADER);
    StyledMultiShape2D* ms = c->addLayer(new StyledMultiShape2D(c, s));
    ms->fillRectangle(10,10, width-20,height-20, grail::black);
    cout << width << "," << height << '\n';
		MultiText *m = c->addLayer(new MultiText(c, s, 100000 * (6 + 3 + 6 + 4)));
    float y = 100;
    y = testOneFontFace(m, "TIMES", y);
    y = testOneFontFace(m, "MATH", y);
    y = testOneFontFace(m, "CENTENNIAL", y);
    y = testOneFontFace(m, "MONO", y);
    y = testOneFontFace(m, "SANS", y);
	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new TestText3(), 1600, 1024);
}
