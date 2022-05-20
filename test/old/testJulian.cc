#include <iostream>
#include "util/JulianDate.hh"
#include <stdio.h>
using namespace std;

void expect(const char filename[], int lineNum, double v, double shouldBe) {
	if (v != shouldBe) {
		cerr << filename << ": " << lineNum << " value should be " << shouldBe << '\n';
	}
}

void expect(const char filename[], int lineNum, int v, int shouldBe) {
	if (v != shouldBe) {
		cerr << filename << ": " << lineNum << " value should be " << shouldBe << '\n';
	}
}

#define EXPECT(v, shouldBe) expect(__FILE__, __LINE__, v, shouldBe)

void testOneDate(JulianDate j, double expected) {
	EXPECT(j.getJulDate(), expected);
	cout << j.getYear() << '\t' << j.getMonth() << '\t' << j.getDay() << '\t' <<
		j.getHour() << '\t' << j.getMinute() << '\t' << j.getSecond() << '\n';
	int32_t year;
	uint32_t mm, dd, hh, min, sec;
	j.extract(&year, &mm, &dd, &hh, &min, &sec);
	cout << year << '\t' << mm << '\t' << dd <<
		'\t' << hh << '\t' << min << '\t' << sec << "\n\n";
}

void testForward(int year, int mon, int day) {
  JulianDate j(year, mon, day);
}

void testHours(JulianDate j) {
  JulianDate t1 = j + 0.25; int h = t1.getHour(); EXPECT(h, 6);
  JulianDate t2 = j + 0.5; int h = t1.getHour(); EXPECT(h, 12);
  JulianDate t3 = j + 0.75; int h = t1.getHour(); EXPECT(h, 18);
  double oneHour = 1.0 / 24;
  
}

int main(int argc, char *argv[]) {
	JulianDate j(2000, 1, 1); 
	for (int i = 0; i < 365; i++, j += 1) {
		testOneDate(j, i);
	}
}
