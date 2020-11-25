#include "util/HighPerfMemAlloc.hh"

void testRegularMalloc(int n) {
	char** p = new char*[n];
	for (int i = 0; i < n; i++)
		p[i] = new char[32];
	for (int i = 0; i < n; i++)
		delete p[i];
	delete[] p;
}

void testMemAlloc(int n) {
	char** p = new char*[n];
	BlockAllocator<32768> block(32);
	for (int i = 0; i < n; i++)
		p[i] = (char*)block.alloc(32);//TODO: wrap in placement new
	delete [] p;
}
	
int main() {
	const int n = 10000000;
	testRegularMalloc(n);

	//testMemAlloc(n);
}
