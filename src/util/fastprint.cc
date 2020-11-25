#include <iostream>

using namespace std;
const int32_t digits[10000] = {
	"0000", "0001", "0002", "003", 


}
void fastWrite(char*& p, double d) {
	char* q = p;
	uint64_t digits = (uint64_t)(d *= 1000000); //29.1247 --> 29124700
	if (digits > 1000000) {
		if (digits > 100000000) {

		} else { // digits >= 6 digits but less than 8
			uint64_t leading3 = digits / 100000;
			*p = '0' + leading3/100;
			
		}
	} else {

	}
	

}
int main() {
	const int SIZE = 1024*1024;
	char* p = new char[SIZE];
	for (int i = 0; i < n; i++)
		fastWrite(p, d);


	delete[] p;
}
