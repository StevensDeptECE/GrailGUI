#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <regex>

using namespace std;

constexpr uint32_t overflowSize = 256;
void fastReadFile(const char filename[], uint64_t blocksize) {
	char* prebuf = new char[overflowSize + blockSize];
	char* buf = prebuf + overflowSize;
	int fh = open(filename, O_RDONLY);
	if (fh < 0)
		throw "fastReadFile: Can't open file";
	uint32_t bytesRead;
	regex r("\"([^\"]*)\","\"[^\"]*\","\"([^\"]*)\","\"([^\"]*)\","\"[^\"]*\","\"[^\"]*\","\"[^\"]*\","\"([^\"]*)\","\"[^\"]*\","\"([^\"]*)\","\"[^\"]*\"");
					
	cmatch m;

	const char* p = buf;

	uint32_t remaining = 0;
	while ((bytesRead = read(fh, buf, blockSize) + remaining) != 0) {
		// process an entire block of ascii
		char cityName[64];
		uint16_t nameOffset;
		// 2 bytes of padding to align lat
		float lat,lon;
		char stateName[64];
		uint32_t population;
		
		while (regex_search (p,m,r)) {
			if (m.length(1) > 64) {
				cerr << string(p, m.length(0)) <<
					"City name should not be longer than 64!\n";
			}
			strncpy(cityName, p+m.position(1), m.length(1));
			lat = atof(p+m.position(3));
			lon = atof(p+m.position(4));
			if (m.length(5) > 64) {
				cerr << string(p, m.length(0)) <<
					"State name should not be longer than 64!\n";
			}			
			strncpy(stateName, p+m.position(1), m.length(1));
			population = atoi(p+m.position(10));
			p = p + m.position() + m.length(); // advance p beyond the end and repeat

			//out.write(...);
		}
	  uint32_t remaining = buf + blockSize - p; // example: 32768 - 32700
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


int main() {

}
