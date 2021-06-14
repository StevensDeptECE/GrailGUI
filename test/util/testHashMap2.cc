#include "util/HashMap.hh"

using namespace std;
int main() {
  HashMap<uint32_t> m(128);
  //torture test to make sure 1-letter names hash well

  char sym[2] = {0};
  for (int i = 0; i < 26; i++) {
    sym[0] = 'a' + i;
    m.add(sym, i);
    sym[0] = 'A' + i;
    m.add(sym, i*2);
  }
  m.hist(); // display histogram to make sure hash map is working well

  uint32_t v;
  for (int i = 0; i < 26; i++) {
    sym[0] = 'a' + i;
    if (!m.get(sym, &v))
      cerr << "Error: symbol " << sym << " not found\n";
    if (v != i)
      cerr << "Error: symbol " << sym << " wrong value: " << v << "\n";
    sym[0] = 'A' + i;
    if (!m.get(sym, &v))
      cerr << "Error: symbol " << sym << " not found\n";
    if (v != 2*i)
      cerr << "Error: symbol " << sym << " wrong value: " << v << "\n";
  }
  const char* symbolsNotInMap[] = {
    "$", "%", "^", "<", ">", ".", "0", "1", "2", "3", "4", "5", "6",
    "test", "yoho", "ab", "ba", "abc", "test2", "verylongname"
    };
  for (int i = 0; i < sizeof(symbolsNotInMap)/sizeof(char*); i++) {
    if (m.get(symbolsNotInMap[i], &v))
      cerr << "Error: found erroneous symbol: " << symbolsNotInMap[i] << '\n';
  }
}