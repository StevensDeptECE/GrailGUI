#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <regex>
#include "util/BLHashMap.hh"
#include "maps/MapNames.hh"

using namespace std;

constexpr uint32_t overflowSize = 256;
void fastReadFile(const char filename[], uint64_t bufferSize) {
  uint32_t itemCount = 10000;
  uint32_t symbolCapacity = 100000;
  BLHashMap<NamedLocationData> mapLocDict = BLHashMap<NamedLocationData>(symbolCapacity, itemCount, itemCount);

	char* prebuf = new char[overflowSize + bufferSize];
	char* buf = prebuf + overflowSize;
	int fh = open(filename, O_RDONLY); //| O_BINARY);
	if (fh < 0)
		throw "fastReadFile: Can't open file";
	uint32_t bytesRead;
	regex r("\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"([^\"]*)\",\"[^\"]*\",\"[^\"]*\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\"");
					
	cmatch m;

	const char* p = buf;

	uint32_t remaining = 0;
	while ((bytesRead = read(fh, buf, bufferSize) + remaining) != 0) {
		// process an entire block of ascii
		char cityName[32];
		uint32_t nameOffset = 0;
		// 2 bytes of padding to align lat
		float lat,lon;
		char stateName[21];
		uint32_t population;
		
		while (regex_search (p,m,r)) {
			if (m.length(1) > 32) {
				cerr << string(p, m.length(0)) <<
					"City name should not be longer than 64!\n";
			}
			strncpy(cityName, p+m.position(1), m.length(1));
      cityName[m.length(1)] = '\0';
			lat = atof(p+m.position(2));
			lon = atof(p+m.position(3));
			if (m.length(5) > 21) {
				cerr << string(p, m.length(0)) <<
					"State name should not be longer than 64!\n";
			}			
			strncpy(stateName, p+m.position(4), m.length(4));
      stateName[m.length(4)] = '\0';
			population = atoi(p+m.position(5));

      cerr << cityName << "\t" << stateName << "\t" << lat << "\t" << lon << "\t" << population << "\n";
      // TODO: cityName needs to be unique.... otherwise data gets replaced
      mapLocDict.add(cityName, NamedLocationData{
        nameOffset, stateName, lat, lon, population});

      uint16_t tempAdvance = m.position() + m.length();
			p = p + tempAdvance; // advance p beyond the end and repeat
      nameOffset = nameOffset + tempAdvance;
			//out.write(...);
		}
	  uint32_t remaining = buf + bufferSize - p; // example: 32768 - 32700
		if (remaining > overflowSize)
			cerr << "Failing on line with " << remaining << " bytes left "
					 << p << endl;
		else {
			// copy the bytes back
			memcpy(buf - remaining, p, remaining);
			p = buf - remaining;
		}
	}
	mapLocDict.writeFile("xp/data/uscities.bdl");
	delete[] prebuf;
	close(fh);
}


int main() {
  fastReadFile("xp/data/uscities.csv", 1024*32);
}
