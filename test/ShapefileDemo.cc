#include <sstream>

#include "opengl/Shapefile.hh"

using namespace std;

//TODO: Draw county map such as in MapDemo
void init() {
  Shapefile counties = Shapefile("res/us_counties/USA_Counties.shp");
  counties.init();

  double xMax, xMin;

  vector<ESRIShape*> shapes = ESRIShape::convertSHPObjects(counties.getShapeVector());
  stringstream buf;
  for (ESRIShape* shape : shapes) {
  }
}

int main() {}