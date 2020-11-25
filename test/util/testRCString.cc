#include "util/RCString.hh"

void f() {
	RCString s1("testing testing 123");
	RCString s2(s1);
	RCString s3 = "abc";
	s2 = s3;
}

void g(int n) {
	for (int i = 0; i < n; i++)
		RCString s("testing testing 123");
}

int main() {
	uint32_t m = RCString::setMark();
	f();
	RCString::freeToMark(m); // all strings gone
	const int n = 10000000;
	g(n);
	RCString::freeToMark(m); // all strings gone
}	
	
