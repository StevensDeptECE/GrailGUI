#include "xp/Cities.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

template<typename T>
void load(std::vector<T>& mylist, const char filename[]) {
  ifstream f(filename);
  char buf[4096];
  while (g.getline(buf)) {
    istringstream line(buf);
    mylist.push_back(T(line));    
  }
}

class Country {
    uint32_t nameOffsetASCII; // location of the name within BLHashMap string table
    uint32_t nameOffset;      // location of the name with 8-bit chars
    double lat, lon; // base lat/lon for country, all locations are single precision relative
    Country(ifstream& f) {
        nameOffsetASCII = f.tellg();
        f >> names;
        nameOffset = f.tellg();
        f >> names;

    }
}

Cities Cities::convertFromASCII : BLHashMap() {
    vector<BlockMapLoader::RegionContainer> regionContainers;
    vector<BlockMapLoader::Country> countries;
}