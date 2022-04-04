#include <stdlib.h>

#include <iostream>
#include <string>

#include "data/BlockMapLoader2.hh"
#include "opengl/GLWin.hh"
#include "util/Benchmark.hh"

using namespace std;
using namespace grail::utils;

string getFile(const char defaultDir[], const char defaultFilename[], int argc,
               char *argv[]) {
  const char *grail = getenv("GRAIL");
  if (grail == nullptr || defaultDir == nullptr)
    return std::string(argc > 1 ? argv[1] : defaultFilename);
  std::string dir = std::string(grail) + std::string(defaultDir);
  return dir + (argc > 1 ? argv[1] : defaultFilename);
}

void loadFromESRITest(const char *shapefile) {
  BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile);
}

void ESRILoadMeanTest(const char *shapefile) {
  float meanx, meany;
  BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile);
  bml.mean(&meanx, &meany);
}

int main(int argc, char **argv) {
  std::string shapefile =
      getFile("/test/res/maps/", "USA_Counties.shp", argc, argv);

  int seg = argc > 2 ? atoi(argv[2]) : -1;
  BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile.c_str());
  // if (seg >= 0) bml.dumpSegment(seg);
  // shapefile = getFile("/test/res/maps/", "uscounties.bml", 0, nullptr);
  float meanx, meany;
  bml.mean(&meanx, &meany);

  CBenchmark<>::benchmarkNoCache(
      "BML load", 1e2, [&]() { loadFromESRITest(shapefile.c_str()); }, true);
  // CBenchmark<>::benchmark("BML mean", 1e2, std::bind(ESRILoadMeanTest,
  // shapefile.c_str()));
}
