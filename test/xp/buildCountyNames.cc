#include <iostream>
#include "util/Benchmark.hh"
#include "mapNames.hh"

using namespace std;
using namespace grail::utils;


const char* FIPsToPostal[100];
BLHashMap<uint32_t> postalToFIPs(200, 128, 128);

#if 0
const char countyName[] = "Alice\nBob\nCalvin\nDov\n";
NamedEntry counties[] = { 
  {0, 5, ENT_COUNTY, FEAT_LOCAL, 0}, // Alice
  {6, 3, ENT_COUNTY, FEAT_LOCAL, 1}, // Bob
  {10, 6, ENT_COUNTY, FEAT_LOCAL, 2}, // Calvin
  {17, 3, ENT_COUNTY, FEAT_LOCAL, 3} // Dov
};
#endif

void build2wayPostal(const char name[], uint32_t FIPs) {
  postalToFIPs.add(name, FIPs);
  FIPsToPostal[FIPs] = name;
}

void buildPostalAbbr() {
  build2wayPostal("AL", 1);
  build2wayPostal("AK", 2);
  build2wayPostal("AZ", 4);
  build2wayPostal("AR", 5);
  build2wayPostal("CA", 6);
  build2wayPostal("CO", 8);
  build2wayPostal("CT", 9);
  build2wayPostal("DE", 10);
  build2wayPostal("DC", 11);
  build2wayPostal("FL", 12);
  build2wayPostal("GA", 13);
  build2wayPostal("HI", 15);
  build2wayPostal("ID", 16);
  build2wayPostal("IL", 17);
  build2wayPostal("IN", 18);
  build2wayPostal("IA", 19);
  build2wayPostal("KS", 20);
  build2wayPostal("KY", 21);
  build2wayPostal("LA", 22);
  build2wayPostal("ME", 23);
  build2wayPostal("MD", 24);
  build2wayPostal("MA", 25);
  build2wayPostal("MI", 26);
  build2wayPostal("MN", 27);
  build2wayPostal("MS", 28);
  build2wayPostal("MO", 29);
  build2wayPostal("MT", 30);
  build2wayPostal("NE", 31);
  build2wayPostal("NV", 32);
  build2wayPostal("NH", 33);
  build2wayPostal("NJ", 34);
  build2wayPostal("NM", 35);
  build2wayPostal("NY", 36);
  build2wayPostal("NC", 37);
  build2wayPostal("ND", 38);
  build2wayPostal("OH", 39);
  build2wayPostal("OK", 40);
  build2wayPostal("OR", 41);
  build2wayPostal("PA", 42);
  build2wayPostal("PR", 72);
  build2wayPostal("RI", 44);
  build2wayPostal("SC", 45);
  build2wayPostal("SD", 46);
  build2wayPostal("TN", 47);
  build2wayPostal("TX", 48);
  build2wayPostal("UT", 49);
  build2wayPostal("VT", 50);
  build2wayPostal("VA", 51);
  build2wayPostal("VI", 78);
  build2wayPostal("WA", 53);
  build2wayPostal("WV", 54);
  build2wayPostal("WI", 55);
  build2wayPostal("WY", 56);
}

// building map dictionary from .dbf
// https://support.esri.com/en/technical-article/000010834
void buildMapDict(const char filename[]) {
  buildPostalAbbr();
  uint32_t itemCount = 10000;
  uint32_t symbolCapacity = 100000;
  BLHashMap<MapEntry> mapDict = BLHashMap<MapEntry>(symbolCapacity, itemCount, itemCount); 
  // loading in from .dbf file
  DBFHandle dbf = DBFOpen(filename, "rb");

  int recordCount = DBFGetRecordCount(dbf);
  int FID = DBFGetFieldIndex(dbf, "FID");  // get index of this field
  int NAME = DBFGetFieldIndex(dbf, "NAME"); // county name
  int STATE_NAME = DBFGetFieldIndex(dbf, "STATE_NAME"); // state name of county
  int STATE_FIPS = DBFGetFieldIndex(dbf, "STATE_FIPS"); // state id
  for (int i = 0; i < recordCount; i++) {
    const int fieldNum = DBFReadIntegerAttribute(dbf, i, FID);
    // array size determined from metadata: max length + 1 (for null)
    char county[24]; 
    strcpy(county, FIPsToPostal[DBFReadIntegerAttribute(dbf, i, STATE_FIPS)]);
    strcat(county, DBFReadStringAttribute(dbf, i, NAME));
    // now county = state abbr + county name. e.g. NYKings
    char state[21]; strcpy(state, DBFReadStringAttribute(dbf, i, STATE_NAME));
    const int stateNum = DBFReadIntegerAttribute(dbf, i, STATE_FIPS);
    cout << fieldNum << '\t'
         << county << '\t'
         << state << '\t'
         << stateNum << '\n';
//    uint32_t symbolSize = mapDict.getSymbolSize();
    mapDict.add(county, MapEntry(
      //symbolSize, (uint32_t)strlen(county), 
      ENT_COUNTY, FEAT_LOCAL, fieldNum));
    if (mapDict.get(state) == nullptr) {
//      symbolSize = mapDict.getSymbolSize();
      mapDict.add(state, MapEntry(
        //symbolSize, (uint32_t)strlen(state), 
        ENT_STATE, FEAT_STATE, stateNum));
    }
  }
  mapDict.writeFile("res/maps/uscounties.bdl");
  DBFClose(dbf);
}

int main() {
  buildMapDict("res/maps/USA_Counties.dbf");
  return 0;
}