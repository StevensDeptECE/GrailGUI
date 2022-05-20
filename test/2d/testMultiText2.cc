#include <numbers>

#include "opengl/GrailGUI.hh"
#include "opengl/MultiText2.hh"
using namespace std;

class TestMultiText2 : public GLWin {
 public:
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Font* f = FontFace::get("TIMES", 40, FontFace::BOLD);
    Style* s = new Style(f, 0, 0, 0, 0, .2, 0.9, GLWin::TEXT_SHADER);

    StyledMultiShape2D* m = c->getGui();
    MultiText2* t = c->addLayer(new MultiText2(c, s, 4096));
    float y = 100;
    const float line = 50;
    const char text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    t->add(10, y += line, f, text, sizeof(text) - 1);
    t->addx(10, y += line, f, 12345);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestMultiText2(), 1600, 1024);
}
