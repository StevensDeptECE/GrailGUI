#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include "data/BlockMapLoader2.hh"
#include "util/Benchmark.hh"
using namespace std;

void loadFromBMLTest(const char* shapefile) {
  BlockMapLoader bml(shapefile);
}

void loadFromBMLTestNoCache(const char* shapefile) {
  std::ofstream ofs("/proc/sys/vm/drop_caches");
  if (!ofs.good() ) {
    cout << "Cannot open file drop_caches\n";
  }
  for (int i = 0; i < 100; i++) {
    BlockMapLoader bml(shapefile);
    ofs << "3" << std::endl;
  }
}

void loadFromBMLRect(const char* shapefile) {
  BlockMapLoader bml(shapefile);
  BoundRect r(-74.5, -74, 40.5, 41);
  int32_t found = -1;
  for (int32_t i = 0; i < bml.getNumRegions(); i = found) {
    found = bml.getRegionWithin(i, r);
    if (found < 0) {
      break;
    }
    //regions[found].
  }
}

void BMLLoadMeanTest(const char* shapefile) {
  float meanx, meany;
  BlockMapLoader bml(shapefile);
  bml.mean(&meanx, &meany);
}

int main(int argc, char* argv[]) {
  string grail = getenv("GRAIL");
  grail += "/test/res/maps/";
  const char* shapefilename = argc > 1 ? argv[1] : "uscounties.bml";

  BlockMapLoader bml((grail + shapefilename).c_str());
  // cout << bml.sum() << '\n';
  //bml.deltaEncode();
  //bml.save((grail + "uscountiesdelta.bml").c_str());
  
  //float meanx, meany;
  //bml.mean(&meanx, &meany);
  
  CBenchmark::benchmark("BML load", 1e2, std::bind(loadFromBMLTest, (grail+shapefilename).c_str()));
  //CBenchmark::benchmark("BML load", 1, std::bind(loadFromBMLTestNoCache, (grail+shapefilename).c_str()));
//  CBenchmark::benchmark("BML mean", 1e2, std::bind(BMLLoadMeanTest, (grail+shapefilename).c_str()));
}
