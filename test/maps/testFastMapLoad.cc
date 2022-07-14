#include <functional>
#include <iostream>
#include <string>

#include "data/BlockMapLoader.hh"
#include "util/Benchmark.hh"
#include "util/Ex.hh"
using namespace std;
using namespace grail::utils;

void loadFromBMLTest(const char* shapefile) { BlockMapLoader bml(shapefile); }

void BMLLoadMeanTest(const char* shapefile) {
  float meanx, meany;
  BlockMapLoader bml(shapefile);
  bml.mean(&meanx, &meany);
}

int main(int argc, char* argv[]) {
  try {
    string grail = getenv("GRAIL");
    grail += "/test/res/maps/";
    const char* shapefilename = argc > 1 ? argv[1] : "uscounties.bml";

    BlockMapLoader bml((grail + shapefilename).c_str());
    // cout << bml.sum() << '\n';
    // bml.deltaEncode();
    // bml.save((grail + "uscountiesdelta.bml").c_str());
    float meanx, meany;
    bml.mean(&meanx, &meany);
#if 0
//TODO: example of loading segments so they can be tessellated
    uint32_t numSegs = bml.getNumSegments();
    for (uint32_t i = 0; i < numSegs; i++) {
      const BlockMapLoader::Segment* seg = bml.getSegment(i);
    }
#endif

    CBenchmark<>::benchmarkNoCache(
        "BML load", 1e2,
        [&]() { loadFromBMLTest((grail + shapefilename).c_str()); }, true);
    CBenchmark<>::benchmarkNoCache(
        "BML mean", 1e2,
        [&]() { BMLLoadMeanTest((grail + shapefilename).c_str()); }, true);
  } catch (const std::exception& e) {
    cerr << e.what() << endl;
  } catch (Ex e) {
    cerr << e << endl;
  } catch (...) {
    cerr << "Exception not caught" << endl;
  }
}
