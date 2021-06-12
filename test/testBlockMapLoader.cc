#include "opengl/BlockMapLoader.hh"

int main(int argc, char* argv[]) {
  const char* shapefilename =
      argc > 1 ? argv[1] : "res/us_counties/USA_Counties.shp";

  BlockMapLoader bml(shapefilename, "ESRI");
  bml.dumpSegment(38);
  bml.save("uscounties.bml");
}
