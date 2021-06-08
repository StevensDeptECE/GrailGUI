#include <sstream>

#include "opengl/Shapefile.hh"

using namespace std;

int main() {
  // Load shapefile and load shape objects
  Shapefile counties = Shapefile("res/us_counties/USA_Counties.shp");
  counties.init();

  // Convert shape objects to our ESRIShape class
  vector<unique_ptr<ESRIShape>> shapes = ESRIShape::convertSHPObjects(counties.getShapeVector());
  stringstream buf;

  // Load points into string buffer
  vector<ESRIPoint> points;
  for (auto const& shape : shapes) {
    points = shape->getPoints();
    for (auto const i : points) {
      buf << i.x << " " << i.y << "\n";
    }
    //buf << points.size() << "\n";
  }

  // Print String buffer
  cout << buf.rdbuf();
}