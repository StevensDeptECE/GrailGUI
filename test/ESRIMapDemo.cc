#include <algorithm>

#include "opengl/GrailGUI.hh"
#include "opengl/Shapefile.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
 public:
  TestMultiShape() : GLWin(0x000000, 0xCCCCCC, "ESRI Shapefile Map Demo") {}

  void transform(ESRIPoint pt, double shiftX, double shiftY, double scaleX, double scaleY) {
    pt.x = pt.x * scaleX + shiftX;
    pt.y = pt.y * scaleY + shiftY;
  }

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    Shapefile counties = Shapefile("res/us_counties/USA_Counties.shp");
    counties.init();

    // Convert shape objects to our ESRIShape class
    vector<unique_ptr<ESRIShape>> shapes = ESRIShape::convertSHPObjects(counties.getShapeVector());

    vector<vector<ESRIPoint>> shapeVector(shapes.size());
    for (int i = 0; i < shapes.size(); i++) {
      shapeVector[i] = shapes[i]->getPoints();
    }

    int xSize = 1920;
    int ySize = 1080;

    double* maxBounds = counties.getMaxBounds();
    double* minBounds = counties.getMinBounds();
    double shiftX = -minBounds[0] * xSize / (maxBounds[0] - minBounds[0]);
    double shiftY = -minBounds[1] * ySize / (maxBounds[1] - minBounds[1]);
    double scaleX = xSize / (maxBounds[0] - minBounds[0]);
    double scaleY = ySize / (maxBounds[1] - minBounds[1]);

    for (const auto& shape : shapeVector) {
      for (const auto& point : shape) {
        transform(point, shiftX, shiftY, scaleX, scaleY);
      }
      gui->drawPolygon(shape, black);  //TODO: Crashes
    }
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestMultiShape(), 1920, 1080);
}
