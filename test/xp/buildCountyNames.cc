#include <iostream>
#include <fstream>
#include <random>
#include "util/BLHashMap.hh"
#include "data/BlockMapLoader2.hh"
#include "libshape/shapefil.h"
#include "util/FileUtil.hh"
#include "util/Benchmark.hh"
#include <cstring>

using namespace std;
using namespace grail::utils;

struct NamedEntry {
    uint32_t nameOffset;  // starting byte of the name;
    uint8_t len;          // length of the name (< 256 bytes)
    uint8_t entityType;   // entity cast as 8 bit number
    uint8_t type;         // feature type of name (could be used to choose font)
    uint32_t offset;      // if entityType=REGION_CONTAINER, then offset into
                          // regionContainers, ...
  };

enum class MapFeatureType {
  LOCAL,
  STATE,
  COUNTRY,
  CITY,
  ROAD,
  TRAIN,
  RIVER
};

#if 0
const char countyName[] = "Alice\nBob\nCalvin\nDov\n";
const uint8_t FEAT_LOCAL = (uint8_t)MapFeatureType::LOCAL;
const uint8_t ENT_COUNTY = (uint8_t)BlockMapLoader::EntityType::REGION;
NamedEntry counties[] = { 
  {0, 5, ENT_COUNTY, FEAT_LOCAL, 0},
  {6, 3, ENT_COUNTY, FEAT_LOCAL, 1},
  {10, 6, ENT_COUNTY, FEAT_LOCAL, 2},
  {17, 3, ENT_COUNTY, FEAT_LOCAL, 3}
};
#endif

// building map dictionary from .dbf
void buildMapDict(const char filename[]) {
  DBFHandle dbf = DBFOpen(filename, "rb");

  int recordCount = 10; //DBFGetRecordCount(dbf);
  int FID = DBFGetFieldIndex(dbf, "FID");  // get index of this field
  int NAME = DBFGetFieldIndex(dbf, "NAME"); // 
  int STATE_NAME = DBFGetFieldIndex(dbf, "STATE_NAME");
  for (int i = 0; i < recordCount; i++) {
    cout << DBFReadIntegerAttribute(dbf, i, FID) << '\t'
         << DBFReadStringAttribute(dbf, i, NAME) << '\t'
         << DBFReadStringAttribute(dbf, i, STATE_NAME) << '\n';
  }
  DBFClose(dbf);
}

int main() {
  buildMapDict("");
  return 0;
}