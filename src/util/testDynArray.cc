#include "util/DynArray.hh"
using namespace std;
int main() {
	DynArray<int> a(10);
	for (int i = 0; i < 10; i++)
		a.add(i);
	cout << a;

	DynArray<string> b(10);
	b.add("test");
	b.add("foo");
	b.add("bar");
	cout << b << '\n';
	for (uint32_t i = 0; i < b.size(); i++)
		b[i] += "a";
	cout << b.removeEnd() << '\n';
	for (uint32_t i = 0; i < b.size(); i++)
		cout << b[i];
	a[3] ++;
  a.removeEnd();
	
	for (int i = 0; i < a.size(); i++)
		cout << a[i];
>>>>>>> 78e51ddef82b35b6599ab6ae0acd7a7182c919b5
}
