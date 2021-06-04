#include "opengl/Shapefile.hh"

using namespace std;

int main() {
  Shapefile counties = Shapefile("res/us_counties/USA_Counties.shp");
  counties.init();
  vector<ESRIShape*> shapes = ESRIShape::convertSHPObjects(counties.getShapeVector());
  vector<vector<double>> points = shapes[0]->dumpPoints();
  for (int i = 0; i < 100; i++) {
    std::cout << "(" << points[i][0] << ", " << points[i][1] << ")\n";
  }
}