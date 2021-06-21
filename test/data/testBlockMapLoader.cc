#include <stdlib.h>

#include <string>

#include "data/BlockMapLoader.hh"
#include "opengl/GLWin.hh"

std::string getFile(const char defaultDir[], const char defaultFilename[],
                    int argc, char *argv[]) {
  const char *grail = getenv("GRAIL");
  if (grail == nullptr || defaultDir == nullptr)
    return std::string(argc > 1 ? argv[1] : defaultFilename);
  std::string dir = std::string(grail) + std::string(defaultDir);
  return dir + (argc > 1 ? argv[1] : defaultFilename);
}

int main(int argc, char **argv) {
  std::string shapefile =
      getFile("/test/res/maps/", "USA_Counties.shp", argc, argv);

  int seg = argc > 2 ? atoi(argv[2]) : -1;
  BlockMapLoader bml(shapefile.c_str(), "ESRI");
  if (seg >= 0) bml.dumpSegment(seg);
  shapefile = getFile("/test/res/maps/", "uscounties.bml", 0, nullptr);
  bml.save(shapefile.c_str());
}
