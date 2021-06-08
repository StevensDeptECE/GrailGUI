#include <algorithm>

#include "opengl/GrailGUI.hh"
#include "opengl/Shapefile.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
 public:
  TestMultiShape() : GLWin(0x000000, 0xCCCCCC, "ESRI Shapefile Map Demo") {}

  void transform(ESRIPoint& pt, double shiftX, double shiftY, double scaleX, double scaleY) {
    pt.x = pt.x * scaleX + shiftX;
    pt.y = pt.y * scaleY + shiftY;
  }

  vector<float> flatten(vector<ESRIPoint>& vec) {
    vector<float> flatvec(vec.size() * 2);
    for (int i = 0, j = 0; i < vec.size(); i++, j += 2) {
      flatvec[j] = (float)vec[i].x;
      flatvec[j + 1] = (float)vec[i].y;
    }
    return flatvec;
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
    double shiftX = 1000 - minBounds[0] * xSize / (maxBounds[0] - minBounds[0]);
    double shiftY = 500 + ySize - (minBounds[1] * ySize / (maxBounds[1] - minBounds[1]));
    double scaleX = 2.5 * xSize / (maxBounds[0] - minBounds[0]);
    double scaleY = -ySize / (maxBounds[1] - minBounds[1]);

    //double xMin = -180, xMax = 100;
    //for (auto& shape : shapeVector) {
    //int j = 0;
    //for (int i = 0; i < shape.size(); i++) {
    //if (shape[i].x >= xMin && shape[i].x <= xMax) {
    //shape[j++] = shape[i];
    //}
    //}
    //shape.resize(j);
    //}

    for (auto& shape : shapeVector) {
      for (auto& point : shape) {
        transform(point, shiftX, shiftY, scaleX, scaleY);
      }
      vector<float> flatvec = flatten(shape);
      gui->drawPolygon(flatvec, black);
    }
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestMultiShape(), 1920, 1080);
}
