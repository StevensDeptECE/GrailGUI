#include "csp/IPV4Socket.hh"
#include "opengl/GrailGUI.hh"
#include "xdl/Renderer.hh"
#include "xdl/std.hh"
using namespace std;

class TestGenericTable : public GLWin {
 private:
  MultiText* t;
  const XDLType* xdl;
  Canvas* c;
  StyledMultiShape2D* m;
  Renderer* renderer;

 public:
  void init() override {
    const char* ip = "127.0.0.1";
    int port = 8000;
    uint32_t req = 0;

    IPV4Socket s(ip, port);
    s.send(1);  // request page 1 which is a list of stock
    Buffer& in = s.getIn();
    in.displayRawRead();
    xdl = XDLType::read(in);  // First read the metadata

    float screenX = 0, screenY = 30, screenW = 0, screenH = 0;
    float objX, objY, objW,
        objH;  // total size of graphical object which may exceed screen size
    renderer = nullptr;  // TODO: make one!
#if 0
		renderer = getViewer(xdl, c, t, m,
											 Renderer::ORIGIN,
											 //											 objX, objY, objW, objH, automatically compute?
											 screenX, screenY, screenW, screenH); // create an appropriate viewer class based on xdl type
#endif
    const Style* style = getDefaultStyle();
    const Font* f = style->f;
    c = currentTab()->getMainCanvas();
    m = c->addLayer(new StyledMultiShape2D(c, style));
    t = c->addLayer(new MultiText(c, style, 8192));
  }
  void render() override { renderer->display(); }
  void update() override {
    renderer.update(Renderer::Dir::DOWN);  // move down and redraw new page with
                                           // same viewport
    // if you want to change the viewer to draw a different area of the screen?
    //		viewer.setViewport(screenx, screeny, screenw, screenh);
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestGenericTable()); }
