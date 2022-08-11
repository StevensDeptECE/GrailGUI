#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <regex>
#include <fstream>
#include "util/BLHashMap.hh"
#include "maps/MapNames.hh"

using namespace std;

constexpr uint32_t overflowSize = 256;
void fastReadFile(const char filename[], uint64_t bufferSize) {
  uint32_t itemCount = 10000;
  // 100000 symbols was not big enough, overflowed into nodes before the hashmap was able to grow
  uint32_t symbolCapacity = 300000;
  BLHashMap<NamedLocationData> mapLocDict = BLHashMap<NamedLocationData>(symbolCapacity, itemCount, itemCount);

	char buf[1024];
	ifstream f(filename);
	if (!f.good())
		throw "fastReadFile: Can't open file";
	regex r("\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"([^\"]*)\",\"[^\"]*\",\"[^\"]*\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\"");
	cmatch m;

  uint32_t nameOffset = 0;
	while ((f.getline(buf, sizeof(buf)))) {
		// process an entire block of ascii
		// char cityName[32];
		// 2 bytes of padding to align lat
		float lat,lon;
		// char stateName[21];
		uint32_t population;
    char locationName[54];
    uint32_t tempOffset;
		
		if (regex_search (buf,m,r)) {
      tempOffset = 0;
			if (m.length(1) > 32) {
				cerr << string(buf, m.length(0)) <<
					"City name should not be longer than 64!\n";
			}
			strncpy(locationName, buf+m.position(1), m.length(1));
      // cityName[m.length(1)] = '\0';
      tempOffset += m.length(1);
      strncpy(locationName + tempOffset, ", ", 2);
      tempOffset += 2;
      
			lat = atof(buf+m.position(2));
			lon = atof(buf+m.position(3));
			if (m.length(5) > 21) {
				cerr << string(buf, m.length(0)) <<
					"State name should not be longer than 64!\n";
			}			
			strncpy(locationName + tempOffset, buf+m.position(4), m.length(4));
      tempOffset += m.length(4);
      locationName[tempOffset] = '\0';
			population = atoi(buf+m.position(5));

      cerr << locationName << "\t" << lat << "\t" << lon << "\t" << population << "\n";
      mapLocDict.add(locationName, NamedLocationData{
        nameOffset, lat, lon, population});

      nameOffset = nameOffset + m.position() + m.length();
			//out.write(...);
    }
  }
	mapLocDict.writeFile("xp/data/uscities.bdl");
}


int main() {
  fastReadFile("xp/data/uscities.csv", 1024*32);
}
