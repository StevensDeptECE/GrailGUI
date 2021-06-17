#include <stdlib.h>

#include "data/BlockMapLoader.hh"

int main(int argc, char* argv[]) {
  const char* shapefilename =
      argc > 1 ? argv[1] : "res/us_counties/USA_Counties.shp";
  int seg = argc > 2 ? atoi(argv[2]) : -1;
  BlockMapLoader bml(shapefilename, "ESRI");
  if (seg >= 0) bml.dumpSegment(seg);
  bml.save("uscounties.bml");
}
