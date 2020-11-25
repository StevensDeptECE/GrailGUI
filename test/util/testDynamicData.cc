#include <iostream>
#include "datatype.hh"
using namespace std;

void buildSampleData(DataType t, char* dest, const char* src) {
  switch (t) {
	case DataType::U32:
		*(uint32_t*)dest = *(uint32_t*)src;
		dest += sizeof(uint32_t); src += sizeof(uint32_t);
		break;

	}

}


int main() {
	uint32_t val = 123;
	char buf[128] = {0};
	buildSampleData(DataType::U32, buf, (const char*)&val);

	for (int i = 0; i < 8; i++)
		cout << (int)buf[i] << '\n';
}
