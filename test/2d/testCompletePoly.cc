#include "grail/Grail.hh"
#include "opengl/primitivesV2/Colors.hh"
using namespace std;

class SimpleDemo : public GLWin {
 public:
  SimpleDemo(uint32_t w, uint32_t h) : Grail(w, h) {}
  void init() {
    Style* s = getDefaultStyle();
    Canvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(s));
    const float gridSize = 300;
    int n = 4;
    for (float x = 0; x < 3; x++)
      for (float y = 0; y < 3; y++, n++) {
        m->drawCompletePolygon(x * gridSize, y * gridSize, gridSize, gridSize,
                               n, grail::red);
      }
    currentTab()->addButton(s, "testing", 500, 200, 100, 50);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new SimpleDemo(), 1024, 1024);
}
