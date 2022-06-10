#include <iostream>
#include "util/BLHashMap.hh"
#include "data/BlockMapLoader2.hh"
#include "libshape/shapefil.h"
#include "util/FileUtil.hh"
#include "util/Benchmark.hh"

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

const uint8_t FEAT_LOCAL = (uint8_t)MapFeatureType::LOCAL;
const uint8_t FEAT_STATE = (uint8_t)MapFeatureType::STATE;
const uint8_t ENT_COUNTY = (uint8_t)BlockMapLoader::EntityType::REGION;
const uint8_t ENT_STATE = (uint8_t)BlockMapLoader::EntityType::REGION_CONTAINER;

#if 0
const char countyName[] = "Alice\nBob\nCalvin\nDov\n";
NamedEntry counties[] = { 
  {0, 5, ENT_COUNTY, FEAT_LOCAL, 0}, // Alice
  {6, 3, ENT_COUNTY, FEAT_LOCAL, 1}, // Bob
  {10, 6, ENT_COUNTY, FEAT_LOCAL, 2}, // Calvin
  {17, 3, ENT_COUNTY, FEAT_LOCAL, 3} // Dov
};
#endif

// building map dictionary from .dbf
// https://support.esri.com/en/technical-article/000010834
void buildMapDict(const char filename[]) {
  uint32_t itemCount = 10000;
  uint32_t symbolCapacity = 1000000;
  BLHashMap<NamedEntry> mapDict = BLHashMap<NamedEntry>(symbolCapacity, itemCount, itemCount); 
  // loading in from .dbf file
  DBFHandle dbf = DBFOpen(filename, "rb");

  int recordCount = 30; //DBFGetRecordCount(dbf);
  int FID = DBFGetFieldIndex(dbf, "FID");  // get index of this field
  int NAME = DBFGetFieldIndex(dbf, "NAME"); // county name
  int STATE_NAME = DBFGetFieldIndex(dbf, "STATE_NAME"); // state name of county
  int STATE_FIPS = DBFGetFieldIndex(dbf, "STATE_FIPS"); // state id
  for (int i = 0; i < recordCount; i++) {
    const int fieldNum = DBFReadIntegerAttribute(dbf, i, FID);
    // array size determined from metadata: max length + 1 (for null)
    char county[22]; strcpy(county, DBFReadStringAttribute(dbf, i, NAME));
    char state[21]; strcpy(state, DBFReadStringAttribute(dbf, i, STATE_NAME));
    const int stateNum = DBFReadIntegerAttribute(dbf, i, STATE_FIPS);
    cout << fieldNum << '\t'
         << county << '\t'
         << state << '\t'
         << stateNum << '\n';
    mapDict.add(county, NamedEntry(0, 0, ENT_COUNTY, FEAT_LOCAL, fieldNum));
    // TODO: what to do about counties with the same name?
    mapDict.add(state, NamedEntry(0, 0, ENT_STATE, FEAT_STATE, stateNum));
  }
  
  DBFClose(dbf);
}

int main() {
  buildMapDict("res/maps/USA_Counties.dbf");
  return 0;
}