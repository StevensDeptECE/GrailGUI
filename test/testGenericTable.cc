#include "opengl/GrailGUI.hh"
#include "csp/IPV4Socket.hh"
#include "xdl/std.hh"
using namespace std;

class GenericTable : public GLWin {
 public:
  void init() {
  	const char* ip = "127.0.0.1";
	  int port = 8000;
	  uint32_t req = 0;
    XDLType::classInit(); //TODO: Figure out a way to do this better!! 
  
  	IPV4Socket s(ip, port);
    s.send(req);
	  Buffer& in = s.getIn();
    in.displayRawRead();
	  const Struct* st = XDLType::read(in);// First read the metadata

    XDLType::classCleanup();
    Style* style = getDefaultStyle();
    const Font* f = style->f;
    Canvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(style));
    MultiText* t = c->addLayer(new MultiText(style, 8192));
    float x = 0, y = 30;
    float w,h;
    const XDLType* p = st->getMemberType(0);
    p->display(in, c, style, x, y, &w, &h);
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new GenericTable());
}
