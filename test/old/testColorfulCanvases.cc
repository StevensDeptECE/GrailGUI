#include "grail/Grail.hh"
using namespace std;


void init(Grail* g) {
	const uint32_t w = GLWin::Width, h = GLWin::Height;
  Style* s = g->getDefaultStyle();
	Canvas* c = g->cw->getMainCanvas();
	MultiShape2D* m = c->addLayer(new MultiShape2D(s));
	const int n = 8;
	uint32_t blockWidth = w/n, blockHeight = h/n;
	float red = 0;
	float blue = 0;
  Font* font = g->getDefaultFont();
	for(int32_t x = 0; x<n;x++){
		red += 1.0f / (n-1);
		blue = 0;
		for(uint32_t y = 0; y<n;y++){
			blue += 1.0f / (n-1);
			Style* r = new Style(font,10,10,0,0,0,red,blue,0);
      r->setShaderIndex(CompiledWebWindow::COMMON_SHADER);
			Canvas* c = g->cw->addCanvas(r, x*blockWidth, y*blockHeight, blockWidth, blockHeight);
      MultiShape2D* layer = c->addLayer(new MultiShape2D(r));
			layer->fillRoundRect(50,50,w-100,h-100,100,100);
		}
	}
#if 0
	Canvas*c = cw->getCanvas(1);
	Style* white = new Style("Arial", 10,10, 0,0,0,  1,1,1);
	white->setLineWidth(1);
	Style* black = new Style("Arial", 10,10, 0,0,0,  0,0,0);
	black->setLineWidth(5);
	MultiShape2D* ms = c->addLayer(new MultiShape2D(white));
	ms->fillRectangle(0,0,0.5,0.5);
	ms->drawPolygon(-.7,0, 0.2,0.2, 5);
	ms->drawPolygon(-.4,0, 0.2,0.2, 6);
	ms->drawPolygon(-.1,0, 0.2,0.2, 3);
	ms->drawCompletePolygon(.4,0, 0.4,0.25, 6);
	
	c = cw->getCanvas(8);
	ms = c->addLayer(new MultiShape2D(black));
	ms->fillRectangle(0,0,0.5,0.5);
	ms->fillPolygon(0,0,0.8,0.5, 5);
#endif
}

int main(int argc, char *argv[]) {
  return Grail::init(1024, 1024, init, nullptr, "output.cml");
}

