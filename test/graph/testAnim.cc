#include "grail/Grail.hh"
#include "opengl/Cursor.hh"
#include "visualcs/MatrixGraph.hh"
#include "visualcs/CSRGraph.hh"
#include "opengl/primitivesV2/GraphView.hh"
#include <malloc.h>
#include <unistd.h>
using namespace std;

void callback(uint32_t src, uint32_t dest) {
	usleep(1000000);
	//TODO:	Grail::setDirty(); //gv->updateVert(dest, 2);
}

class GraphViewer : public GLWin {
private:
	CSRGraph<uint64_t, uint64_t, float> *csr;
	GraphView *gv;
public:
	GraphViewer(uint32_t w, uint32_t h) : Grail(w,h) {}
	void init() {
		Canvas *c = currentTab()->getMainCanvas();
		const Font *f = getDefaultFont();
		Style *style = getDefaultStyle();
		glm::vec4 defaultColor(1,0,0,1);
		
		// csr = new CSRGraph<uint64_t, uint64_t, float>("email-Eu-core.txt_beg_pos.bin", "email-Eu-core.txt_csr.bin", nullptr);
		csr = new CSRGraph<uint64_t, uint64_t, float>("res/toy.dat_beg_pos.bin", "res/toy.dat_csr.bin", nullptr);
		gv = new GraphView(*csr, c, style, defaultColor);
		gv->addColor(0,0,0);
		gv->addColor(0,0,1);
		gv->addColor(0,1,1);
		circleLayout(gv, c->getWidth()/2,c->getHeight()/2, 900.0, csr->getV(), 0, 0);
		c->addLayer(gv);
	}

	void run() {
		csr->dfs(0, callback);
	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new GraphViewer(), 1450, 1150);
}
