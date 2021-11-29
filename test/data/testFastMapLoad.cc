#include <iostream>
#include <string>
#include <functional>

#include "data/BlockMapLoader2.hh"
#include "util/Benchmark.hh"
using namespace std;

void loadFromBMLTest(const char* shapefile) {
  BlockMapLoader bml(shapefile);

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
  float meanx, meany;
  bml.mean(&meanx, &meany);
  
  CBenchmark::benchmark("BML load", 1e2, std::bind(loadFromBMLTest, (grail+shapefilename).c_str()));
  CBenchmark::benchmark("BML mean", 1e2, std::bind(BMLLoadMeanTest, (grail+shapefilename).c_str()));
}
