#include "opengl/GrailGUI.hh"
#include "opengl/StyledMultiShape25D.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
 public:
  TestMultiShape() : GLWin(0x000000, 0xCCCCCC, "Test StyledMultiShape") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();

    const Style* s = getDefaultStyle();
    StyledMultiShape25D* gui = c->addLayer(new StyledMultiShape25D(c, s));

    const float boxSize = 100;
    const float drawSize = 90;

    gui->fillRectangle(500, 600, 1, 200, 200, red);
    gui->fillRectangle(550, 650, 1, 200, 200, glm::vec4(0, 0, 0, 0.5f));
    gui->drawRectangle(500, 600, 0, 250, 100, red);
    gui->drawRectangle(550, 650, 0, 250, 100, glm::vec4(0, 0, 0, 0.5f));

    #if 0
    gui->drawRectangle(0, 400, 0, drawSize, drawSize, green);
    gui->drawPolygon(boxSize * 1, 400, 0, drawSize, drawSize, 5, blue);
    gui->drawPolygon(boxSize * 2, 400, 0, drawSize, drawSize, 6, blue);
    gui->drawRoundRect(boxSize * 3, 400, 0, drawSize, drawSize, 20, 20, red);
    gui->drawCircle(boxSize * 4.5, boxSize * 0.5, 0, drawSize / 2, 5, purple);
    gui->drawEllipse(boxSize * 5.5, boxSize * 0.5, 0, drawSize / 2, drawSize * .7,
                     5, purple);
    gui->drawLine(boxSize * 6, 0, 0, boxSize * 8, boxSize, green);
    //	gui->drawCompletePolygon(boxSize*7,0, drawSize, drawSize, 6, purple);

    gui->rectanglePoints(100, 100, 0, 200, 300, green);
    gui->trianglePoints(100, 100, 0, 200, 250, 600, 100, green);
    gui->polygonPoints(boxSize * 1, 0, 0, drawSize, drawSize, 5, green);

    float xy[] = {200, 100, 300, 100, 400, 500, 500, 500};
    gui->drawPolyline(xy, 0, 4, red);
    float xy2[] = {400, 100, 0, 550, 120, 650, 100, 550, 220, 400, 205};
    gui->fillPolygon(xy2, 5, 0, cyan);
    gui->drawPolygon(xy2, 5, 0, pink);
    gui->fillRectangle(0, boxSize, 0, drawSize, drawSize, gray);
    gui->fillPolygon(boxSize * 1, boxSize, 0, drawSize, drawSize, 5, yellow);
    gui->fillPolygon(boxSize * 2, boxSize, 0, drawSize, drawSize, 6, yellow);

    gui->fillRoundRect(boxSize * 3, boxSize, 0, drawSize, drawSize, 20, 20, gray);
    gui->fillCircle(boxSize * 7.5, boxSize * 1.5, 0, drawSize / 2, 5, black);
    gui->fillEllipse(boxSize * 8.5, boxSize * 1.5, 0, drawSize / 2, drawSize * .7,
                     5, purple);

    //    gui->bezierSegmentByPoints(0,0, 100,50, 400,300, 400,350,  10, true);
    vector<double> points;
    points.push_back(600);
    points.push_back(250);
    points.push_back(250);
    points.push_back(550);
    points.push_back(250);
    points.push_back(250);
    points.push_back(600);
    points.push_back(550);
    gui->spline(points, 100, purple);

    #endif
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestMultiShape()); }
