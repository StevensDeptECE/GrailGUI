#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <fstream>
#include <regex>
#include <vector>
#include "util/BLHashMap.hh"
#include "maps/MapNames.hh"
#include "util/Benchmark.hh"

using namespace std;
using namespace grail::utils;

constexpr uint32_t overflowSize = 256;
void loadAndCompare(const char filename[], uint64_t bufferSize) {
  uint32_t itemCount = 10000;
  uint32_t symbolCapacity = 100000;
  NamedLocationData temp(0, "", 0, 0, 0);
  BLHashMap<NamedLocationData> mapLocDict("xp/data/uscities.bdl");

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

      // cerr << cityName << "\t" << stateName << "\t" << lat << "\t" << lon << "\t" << population << "\n";
      
      // CHECKING PART =======================================================
      if (mapLocDict.get(cityName, &temp)) {
        if (!(temp == NamedLocationData(nameOffset, stateName, lat, lon, population)))
          cout << cityName << " data mismatch.\n";
      } else {
        cout << cityName << " not found.\n";
      }

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
	delete[] prebuf;
	close(fh);
}

void testLoadMapBlockDict(const char filename[]) {
  BLHashMap<NamedLocationData> mapLocDict(filename);
}

struct TempNamedLocationData {
  char cityName[32];
  char stateName[21];
  float lat;
  float lon;
  uint32_t population;

  TempNamedLocationData(const char cityName[32], const char stateName[21], float lat, float lon, uint32_t population)
  : lat(lat), lon(lon), population(population) {
    strncpy(this->cityName, cityName, 32);
    strncpy(this->stateName, stateName, 21);
  }
};


void testLoadCity(const char filename[]) {
	char buffer[1024];
	ifstream f(filename);
	regex r("\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"([^\"]*)\",\"[^\"]*\",\"[^\"]*\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\"");
	cmatch m;
	
  char cityName[32];
	uint32_t nameOffset = 0;
		// 2 bytes of padding to align lat
	float lat,lon;
	char stateName[21];
	uint32_t population;	
	vector<TempNamedLocationData> locations;

	while (f.getline(buffer, sizeof(buffer))) {
		if (regex_search (buffer,m,r)) {
			if (m.length(1) > 32) {
				cerr << string(buffer, m.length(0)) <<
					"City name should not be longer than 32!\n";
			}
			strncpy(cityName, buffer+m.position(1), m.length(1));
      cityName[m.length(1)] = '\0';
			lat = atof(buffer+m.position(2));
			lon = atof(buffer+m.position(3));
			if (m.length(5) > 21) {
				cerr << string(buffer, m.length(0)) <<
					"State name should not be longer than 21!\n";
			}
			strncpy(stateName, buffer+m.position(4), m.length(4));
      stateName[m.length(4)] = '\0';
			population = atoi(buffer+m.position(5));
			locations.push_back(TempNamedLocationData(cityName, stateName, lat, lon, population));

//      cerr << cityName << "\t" << stateName << "\t" << lat << "\t" << lon << "\t" << population << "\n";
		}
	}
}

void performTests(const char filename[]) {
  loadAndCompare(filename, 1024*32);

  CBenchmark<>::benchmark(
    "Loading uscities.bdl", 1e3, [&]() { testLoadMapBlockDict("xp/data/uscities.bdl"); });
  CBenchmark<>::benchmark(
    "Test loading city ASCII", 1e2, [&]() { testLoadCity("xp/data/uscities.csv"); });
}

int main() {
  performTests("xp/data/uscities.csv");
  return 0;
}