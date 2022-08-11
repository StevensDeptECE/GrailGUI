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
  // 100000 symbols was not big enough, overflowed into nodes before the hashmap was able to grow
  uint32_t symbolCapacity = 300000;
  NamedLocationData temp(0, 0, 0, 0);
  BLHashMap<NamedLocationData> mapLocDict("xp/data/uscities.bdl");

	char buf[1024];
	ifstream f(filename);
	if (!f.good())
		throw "fastReadFile: Can't open file";
	regex r("\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"([^\"]*)\",\"[^\"]*\",\"[^\"]*\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\",\"([^\"]*)\",\"[^\"]*\"");
	cmatch m;

  uint32_t nameOffset = 0;
  uint16_t mismatchCounter = 0;
  uint16_t notFoundCounter = 0;
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

      // cerr << locationName << "\t" << lat << "\t" << lon << "\t" << population << "\n";

      // CHECKING PART =======================================================
      if (mapLocDict.get(locationName, &temp)) {
        if (!(temp == NamedLocationData(nameOffset, lat, lon, population))) {
          cout << locationName << " data mismatch.\n";
          mismatchCounter++;
          // there will be data mismatches since there are multiple data for the same location
        }
      } else {
        cout << locationName << " not found.\n";
        notFoundCounter++;
      }

      nameOffset = nameOffset + m.position() + m.length();
			//out.write(...);
		}
	}
  cout << "Total mismatches: " << mismatchCounter << '\n';
  cout << "Total not found: " << notFoundCounter << '\n';
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