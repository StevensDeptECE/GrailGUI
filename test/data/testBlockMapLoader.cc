#include <stdlib.h>

#include <string>

#include "data/BlockMapLoader.hh"

int main(int argc, char* argv[]) {
  std::string grail = getenv("GRAIL");
  grail += "/test/res/maps/";
  const char* shapefilename = argc > 1 ? argv[1] : "USA_Counties.shp";

  int seg = argc > 2 ? atoi(argv[2]) : -1;
  BlockMapLoader bml((grail + shapefilename).c_str(), "ESRI");
  if (seg >= 0) bml.dumpSegment(seg);
  bml.save((grail + "uscounties.bml").c_str());
}
