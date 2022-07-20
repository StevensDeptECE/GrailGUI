#include <stdlib.h>

#include <string>

#include <vector>
#include "maps/MapNames.hh"
#include "data/BlockMapLoader.hh"
#include "opengl/GLWin.hh"
using namespace std;

std::string getFile(const char defaultDir[], const char defaultFilename[],
                    int argc, char *argv[]) {
  const char *grail = getenv("GRAIL");
  if (grail == nullptr || defaultDir == nullptr)
    return string(argc > 1 ? argv[1] : defaultFilename);
  string dir = string(grail) + string(defaultDir);
  return dir + (argc > 1 ? argv[1] : defaultFilename);
}

int main(int argc, char **argv) {
  std::string shapefile =
      getFile("/test/res/maps/", "USA_Counties.shp", argc, argv);

  int seg = argc > 2 ? atoi(argv[2]) : -1;
  uint32_t stateCount;
  vector<NamedMapEntry> sortedEntries = BlockMapLoader::buildMapDict("res/maps/USA_Counties.dbf", stateCount);
  BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile.c_str(), sortedEntries, stateCount);
  if (seg >= 0) bml.dumpSegment(seg);
  shapefile = getFile("/test/res/maps/", "uscounties.bml", 0, nullptr);
  bml.save(shapefile.c_str());
}
