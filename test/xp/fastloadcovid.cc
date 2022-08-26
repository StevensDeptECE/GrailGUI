#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <regex>
#include <vector>
#include <unordered_map>
#include "util/FileUtil.hh"

using namespace std;
constexpr uint32_t overflowSize = 256;
//FIPS,Admin2,Province_State,Country_Region,Last_Update,Lat,Long_
//,Confirmed,Deaths,Recovered,Active,Combined_Key,Incident_Rate,Case_Fatality_Ratio


//Alabama,US,2022-01-02 04:32:57,32.3182,-86.9023,896614,16455,,,1.0,18286.358764762088,
//6547917.0,,1.8352379061669792,84000001.0,USA,133544.1554826098,

class COVIDReader {
private:
  struct Location {
    char state[21];
		char county[24];
		float lat, lon;
  };
	struct COVID {
    uint32_t confirmed;   // total confirmed cases for this location
		uint32_t deaths;      // number of deaths due to COVID
		uint32_t recovered;   // undead
		uint32_t active;      // current cases
		float incidentRatio;
		float caseFatality;
  };
	uint64_t bufferSize;
	char* preBuf;
	char* buf;
	std::vector<Location> locations;
	std::unordered_map<uint32_t, uint32_t> fips2uniqueId;
	std::vector<COVID> covidForeachCounty;
  std::regex covidPattern;
  constexpr static uint32_t overflowSize = 256;
  COVIDReader(uint64_t bufferSize) : bufferSize(bufferSize),
	// precompute covid Regex once, why do it repeatedly for each file?
    covidPattern("([^,]*),([^,]*),([^,]*),([^,]*),[^,]*,([^,]*),([^,]*),([^,]*),([^,]*),([^,]*),([^,]*),\"[^\"]*\",([^,]*),([^,]*)\n")
	 {
    preBuf = new char[overflowSize + bufferSize + 1];
		buf = preBuf + overflowSize;
		buf[bufferSize] = '\0'; // make sure buffer is always nul terminated
		covid.reserve(3220); // TODO: maybe discover this, not hardcode?
		int fh = open("01-01-2021.csv", O_RDONLY); //| O_BINARY);
	  if (fh < 0)
  		throw "readFile: Can't open file";
	  	
	}

  void read(const char filename[]) {
		char* buf;
		uint32_t buflen;
		FileUtil::readComplete(&buf, &buflen, filename);
		char* p = strtok(buf, "\n");

		while (p != nullptr) {
			// process an entire block of ascii
			uint32_t nameOffset = 0;
			// 2 bytes of padding to align lat
			uint32_t fips;
			float lat,lon;
			char state[21];
			char county[24];
			char country[64];
			COVID covid;
			float incidentRate;
			float caseFatalityRatio;

			while (regex_search (p,m,covidPattern)) {
				fips = atoi(p + m.position(1));
				strncpy(county, p + m.position(2), sizeof(county)-1);
				county[m.position(2)] = '\0';
				strncpy(state, p + m.position(3), sizeof(state)-1);
				state[m.position(3)] = '\0';
				strncpy(country, p + m.position(4), sizeof(country)-1);
				country[m.position(4)] = '\0';
				lat = float(atof(p + m.position(5)));
				lon = float(atof(p + m.position(6)));

			  covid.active = atoi(p + m.position(7));
			  covid.confirmed = atoi(p + m.position(8));
			  covid.deaths = atoi(p + m.position(9));
			  covid.recovered = atoi(p + m.position(10));
				covid.incidentRatio = float(atof(p + m.position(11)));
				covid.incidentRatio = float(atof(p + m.position(12)));
				if (fips2uniqueId.contains())
				  covidForeachCounty[fips2uniqueId.at(fips)] = covid;
				
				if (fips >= 1001 && fips < 10000) {
					locations[fips-1001] = covid;
				}
			}
			strncpy(cityName, p+m.position(1), m.length(1));
      cityName[m.length(1)] = '\0';
			lat = atof(p+m.position(2));
			lon = atof(p+m.position(3));
			if (m.length(5) > 64) {
				cerr << string(p, m.length(0)) <<
					"State name should not be longer than 64!\n";
			}			
			strncpy(stateName, p+m.position(4), m.length(4));
      stateName[m.length(4)] = '\0';
			population = atoi(p+m.position(5));

      cerr << cityName << "\t" << stateName << "\t" << lat << "\t" << lon << "\t" << population << "\n";
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
