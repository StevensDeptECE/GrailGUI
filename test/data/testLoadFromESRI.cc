#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <string>

#include "data/BlockMapLoader2.hh"
#include "opengl/GLWin.hh"
#include "util/Benchmark.hh"
#include "libshape/shapefil.h"

using namespace std;

string getFile(const char defaultDir[],
                                        const char defaultFilename[], int argc,
                                        char *argv[]) {
  const char *grail = getenv("GRAIL");
  if (grail == nullptr || defaultDir == nullptr)
    return std::string(argc > 1 ? argv[1] : defaultFilename);
  std::string dir = std::string(grail) + std::string(defaultDir);
  return dir + (argc > 1 ? argv[1] : defaultFilename);
}

void loadFromESRITest(const char shapefile[]) {
  SHPHandle shapeHandle = SHPOpen(shapefile, "rb");
  // Load basic information
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;
  //cerr << "nEntities: " << nEntities << endl;


  uint64_t numPoints = 0;
  uint32_t numSegments = 0;
  vector< vector<float> > points(nEntities);
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    vector<float> p;
    for (int j = 0; j < shape->nVertices; j++) {
      p.push_back(shape->padfX[j]);
      p.push_back(shape->padfY[j]);
    }
    SHPDestroyObject(shape);
    points[i] = p;
  }
  SHPClose(shapeHandle);
  uint32_t length = 0;
  for (const auto& p : points)
  length += p.size();
  //cerr << length << '\n';
}

void loadFromESRIInRange(const char shapefile[], const BoundRect& r) {
  SHPHandle shapeHandle = SHPOpen(shapefile, "rb");
  // Load basic information
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;
  //cerr << "nEntities: " << nEntities << endl;

  uint64_t numPoints = 0;
  uint32_t numSegments = 0;
  vector< vector<float> > points(nEntities);
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    vector<float> p;
    bool isWithin = false;
    for (int j = 0; j < shape->nVertices; j++) {
      if (r.contains(shape->padfX[j], shape->padfY[j])) {
        isWithin = true;
        for (int k = 0; k < shape->nVertices; k++) {
          p.push_back(shape->padfX[k]);
          p.push_back(shape->padfY[k]);
        }
        break;
      }
    }
    SHPDestroyObject(shape);
    points[i] = p;
  }
  SHPClose(shapeHandle);
  uint32_t length = 0;
  for (const auto& p : points)
  length += p.size();
  length /= 2;
  cerr << length << '\n';
}

void loadFromESRITestNoCache(const char* shapefile) {
  std::ofstream ofs("/proc/sys/vm/drop_caches");
  if (!ofs.good() ) {
    cout << "Cannot open file drop_caches\n";
  }
  for (uint32_t i = 0; i < 100; i++) {
   BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile);
   ofs << "3" << std::endl;
  }
}

void loadFromESRITestMultiFile(const char* shapefile) {
  std::string base = shapefile;
  base += " ";
  const int end = base.length()-1;
  for (uint32_t i = 0; i < 10; i++) {
    base[end] = '0' + i;
    BlockMapLoader bml = BlockMapLoader::loadFromESRI(base.c_str());
  }
}

void ESRILoadMeanTest(const char* shapefile) {
  float meanx, meany;
  BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile);
  bml.mean(&meanx, &meany);
}

int main(int argc, char **argv) {
  std::string shapefile =
      getFile("/test/res/maps/", "USA_Counties.shp", argc, argv);

  int seg = argc > 2 ? atoi(argv[2]) : -1;
  BlockMapLoader bml = BlockMapLoader::loadFromESRI(shapefile.c_str());
  //if (seg >= 0) bml.dumpSegment(seg);
  //shapefile = getFile("/test/res/maps/", "uscounties.bml", 0, nullptr);
  float meanx, meany;
  bml.mean(&meanx, &meany);

  
  CBenchmark::benchmark("BML load", 1e2, std::bind(loadFromESRITest, shapefile.c_str()));
  BoundRect r(-74, -73, 40.5, 41);
  CBenchmark::benchmark("BML load", 1e2, std::bind(loadFromESRIInRange, shapefile.c_str(), r));
  //CBenchmark::benchmark("BML load no cache", 1, std::bind(loadFromESRITestNoCache, shapefile.c_str()));
  //CBenchmark::benchmark("BML load multi file", 1, std::bind(loadFromESRITestMultiFile, shapefile.c_str()));
  //CBenchmark::benchmark("BML mean", 1e2, std::bind(ESRILoadMeanTest, shapefile.c_str()));
}
