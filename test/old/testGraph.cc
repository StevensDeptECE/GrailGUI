#include "grail/Grail.hh"
#include "opengl/primitivesV2/GraphView.hh"
#include "visualcs/CSRGraph.hh"
#include "visualcs/MatrixGraph.hh"
using namespace std;

class TestGraphViewer : public GLWin {
 public:
  void init() {
    Style* s = getDefaultStyle();
    Canvas* c = currentTab()->getMainCanvas();

    CSRGraph<uint64_t, uint64_t, float> cg("res/email-Eu-core.txt_beg_pos.bin",
                                           "res/email-Eu-core.txt_csr.bin",
                                           nullptr);
    // CSRGraph<uint64_t, uint64_t, float> cg("res/toy.dat_beg_pos.bin",
    // "res/toy.dat_csr.bin", nullptr);

    cg.group();
    GraphView* gv = new GraphView(cg, c, s, glm::vec4(1, 0, 0, 0));

    gv->addColor(1, 0, 0);
    gv->addColor(0, 0, 1);
    gv->addColor(0, 1, 1);
    // circleLayout(gv, 512,512, 300.0, cg.getV(), 0, 0);
    multiCircleLayout(gv, 1024, 1024, 460.0, 40.0, 20, cg.getV(), 0, 0);

    c->addLayer(gv);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestGraphViewer(), 2048, 2048);
}
