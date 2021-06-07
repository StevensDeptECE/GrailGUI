#include <unistd.h>

#include <sstream>

#include "opengl/Shapefile.hh"

using namespace std;

int main() {
  Shapefile counties = Shapefile("res/us_counties/USA_Counties.shp");
  counties.init();
  vector<unique_ptr<ESRIShape>> shapes = ESRIShape::convertSHPObjects(counties.getShapeVector());
  stringstream buf;
  vector<vector<double>> points;
  for (auto const& shape : shapes) {
    points = shape->dumpPoints();
    for (auto const i : points) {
      buf << "(" << i[0] << ", " << i[1] << ")\n";
    }
    buf << points.size() << "\n";
  }
  cout << buf.rdbuf();
  sleep(100);
}