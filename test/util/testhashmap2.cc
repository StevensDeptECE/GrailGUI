#include "util/HashMap.hh"

using namespace std;
int main() {
  HashMap<uint32_t> m(16);
  m.add("a", 5);
  m.add("b", 4);
  m.add("c", -2);
  uint32_t a;
  if (m.get("a", &a))
    cout << "a= " << a<<endl;
  if (m.get("b", &a))
    cout << "b= " << a<<endl;
  if (m.get("c", &a))
    cout << "c= " << a<<endl;
  if (m.get("x", &a))
    cout << "x= " << a<<endl;
}