#include "Repository.hh"

int main() {
	Blob test("my blob");
	Blob copy = test; // calls copy constructor
	Blob copy2(test); // calls copy constructor
	Blob c2("test");
	c2 = test; // this calls operator =
}
