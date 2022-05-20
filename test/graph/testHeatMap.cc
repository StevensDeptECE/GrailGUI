#include "grail/Grail.hh"
#include "opengl/HeatMap.hh"

class TestHeatMap : public GLWin {
 public:
  void init() {
    Canvas* c = currentTab()->getMainCanvas();
    HeatMap* h = new HeatMap(2, 2);
    h->add(0, 0, 0.0, 300, 0, 1.0, 0, 300, 0.5, 300, 300, 0.25);
    h->add(400, 0, 0.8, 700, 0, 0.7, 400, 300, 0.2, 800, 300, 0.4);
    c->addLayer(h);
  }
};
int main(int argc, char* argv[]) {
  return GLWin::init(new TestHeatMap(), 1024, 1024);
}
