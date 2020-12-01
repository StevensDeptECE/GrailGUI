#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
public:
	TestMultiShape(uint32_t w, uint32_t h) :
		GLWin(w, h, 0x000000, 0xCCCCCC, "Test StyledMultiShape") {}
	void init() {
		const Style *s = getDefaultStyle();
		//		const Font *font = getDefaultFont();
		//addCanvas(s, 0, 300, Width, Height-300);
		MainCanvas* c = currentTab()->getMainCanvas();
		//		StyledMultiShape2D *m = c->addLayer(new StyledMultiShape2D(s));
		StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(s));

		const float boxSize = 100;
		const float drawSize = 90;

		m->drawRectangle(0, 400, drawSize, drawSize, green);
		m->drawPolygon(boxSize * 1, 400, drawSize, drawSize, 5, blue);
		m->drawPolygon(boxSize * 2, 400, drawSize, drawSize, 6, blue);
		m->drawRoundRect(boxSize * 3, 400, drawSize, drawSize, 20, 20, red);
		m->drawCircle(boxSize * 4.5, boxSize * 0.5, drawSize / 2, 5, purple);
		m->drawEllipse(boxSize * 5.5, boxSize * 0.5, drawSize / 2, drawSize * .7, 5, purple);
		m->drawLine(boxSize * 6, 0, boxSize * 8, boxSize, green);
		//	m->drawCompletePolygon(boxSize*7,0, drawSize, drawSize, 6, purple);

		m->rectanglePoints(100, 100, 200, 300, green);
		m->trianglePoints(100, 100, 200, 250, 600, 100, green);
		m->polygonPoints(boxSize * 1, 0, drawSize, drawSize, 5, green);

		float xy[] = {200, 100, 300, 100, 400, 500, 500, 500};
		m->drawPolyline(xy, 4, red);
		float xy2[] = {400, 100, 550, 120, 650, 100, 550, 220, 400, 205};
		m->fillPolygon(xy2, 5, cyan);
		m->drawPolygon(xy2, 5, pink);
		m->fillRectangle(0, boxSize, drawSize, drawSize, gray);
		m->fillPolygon(boxSize * 1, boxSize, drawSize, drawSize, 5, yellow);
		m->fillPolygon(boxSize * 2, boxSize, drawSize, drawSize, 6, yellow);
		//TODO: polygons are connecting to previous shapes, filling too much
		m->fillRoundRect(boxSize * 3, boxSize, drawSize, drawSize, 20, 20, gray);
		m->fillCircle(boxSize * 7.5, boxSize * 1.5, drawSize / 2, 5, black);
		m->fillEllipse(boxSize * 8.5, boxSize * 1.5, drawSize / 2, drawSize * .7, 5, purple);

		//    m->bezierSegmentByPoints(0,0, 100,50, 400,300, 400,350,  10, true);
		vector<double> points;
		points.push_back(600);
		points.push_back(250);
		points.push_back(250);
		points.push_back(550);
		points.push_back(250);
		points.push_back(250);
		points.push_back(600);
		points.push_back(550);
		m->spline(points, 100, purple);
	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new TestMultiShape(1024, 1024));
}
