#define _USE_MATH_DEFINES
#include <cmath>

#include "opengl/GrailGUI.hh"
using namespace std;

class TestText3 : public Member {
 public:
  void numericFormatting(MultiText* m, const Font* f, float y) {
    m->add(10, y, f, 123);
    m->addx(150, y, f, M_PI);
    m->addx(400, y, f, 2.5f);
    m->addx(550, y, f, 123456789U);
    m->addx(700, y, f, 123456789012345678ULL);
    y += 20;
    m->addHex0(10, y, f, 0x0034ABCD);
    m->addHex0(150, y, f, 0xABCDEF12);
    // TODO: uint64_t conversion is broken on Windows? so we won't do it for
    // now. m->addHex0(300, y, f, 0xABCDEF1234567890ULL);
    m->addHex0(550, y, f, uint64_t(0xABCDEF1234567890ULL));
    m->addHex0(800, y, f, uint8_t(0xFF));
    m->addHex0(900, y, f, uint16_t(0xBEEF));
  }

  void sampleText2(MultiText* m, const Font* f, float y) {
    static const char s[] =
        "!\"#$%&'()*+"
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    m->add(20, y, f, s, sizeof(s) - 1);
  }

  void testRotatingText(MultiText* m, const char fontFamily[], float x,
                        float y) {
    const Font* font = FontFace::get(fontFamily, 40, 0);
    const char s[] = "test";
    for (int a = 0; a < 360; a += 45)
      m->add(x, y, 50, a * (M_PI / 180), font, s, sizeof(s) - 1);
  }

  float testOneFontFace(MultiText* m, const char fontFamily[], float yStart) {
    const int sizes[] = {40, 30, 20, 10};

    for (auto size : sizes) {
      const Font* font = FontFace::get(fontFamily, size, 0);
      sampleText2(m, font, yStart);
      yStart += size + 4;
    }
    return yStart + 50;
  }

  TestText3(GLWin* w) : Member(w, 0, 0.1) {
    Canvas* c = tab->getMainCanvas();
    uint32_t width = c->getWidth(), height = c->getHeight();

    const Font* f = FontFace::get("TIMES", 16, FontFace::BOLD);

    Style* s = new Style(f, 0, 0, 0, 0, .2, 0.9, GLWin::TEXT_SHADER);

    StyledMultiShape2D* ms = c->addLayer(new StyledMultiShape2D(c, s));
    ms->fillRectangle(10, 10, width - 20, height - 20, grail::black);
    cout << width << "," << height << '\n';
    MultiText* m = c->addLayer(new MultiText(c, s, 100000 * (6 + 3 + 6 + 4)));
    float topRow = 30;
    numericFormatting(m, f, 30);
    testRotatingText(m, "TIMES", 1400, 100);
    float y = 200;
    y = testOneFontFace(m, "TIMES", y);
    y = testOneFontFace(m, "MATH", y);
    y = testOneFontFace(m, "CENTENNIAL", y);
    y = testOneFontFace(m, "MONO", y);
    y = testOneFontFace(m, "SANS", y);
  }
};

void grailmain(int argc, char* argv[], GLWin* w) {
  w->setTitle("Test Text3");
  new TestText3(w);
}
