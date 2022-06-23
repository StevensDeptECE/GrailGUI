#include <string.h>

#include <iostream>
#include <unordered_map>
using namespace std;

void buildHashMap(int n) {
  unordered_map<string, int> m;
  char buf[20];
  for (int i = 0; i < n; i++) {
    buf[0] = 'a' + i % 26;
    buf[1] = 'a' + i / 26 % 26;
    buf[2] = 'a' + i / (26 * 26) % 26;
    sprintf(buf + 3, "%d", i);
    m[buf] = i;
  }
}

void benchmark(const char msg[], void (*f)(int n), int n) {
  clock_t t0 = clock();
  f(n);
  clock_t t1 = clock();
  cout << msg << " " << (t1 - t0) * 1e-6 << '\n';
}

#define bench(f, n) benchmark(#f, f, n)

template <typename Val>
class HashMap {
 private:
  char* symbols;
  char* current;
  class Node {
   public:
    uint32_t offset;
    Val val;
    Node* next;
    Node(uint32_t offset, Val v, Node* next)
        : offset(offset), val(v), next(next) {}
  };
  uint32_t size;
  Node** table;

  uint32_t bytewisehash(const char s[]) const {
    uint32_t i;
    uint32_t sum = s[0] ^ 0xf67392AC;
    for (i = 1; s[i] != '\0'; i++) sum = (sum << 17) + (sum >> 13) + s[i];
    sum ^= i;
    return sum & size;
  }
  static bool hasNoZero(uint32_t v) {
    return (v & 0xff) && (v & 0xff00) && (v & 0xff0000) && (v & 0xff000000);
  }
  static bool hasNoZero(uint64_t v) {
    return hasNoZero(uint32_t(v >> 32)) && hasNoZero(uint32_t(v));
  }

  static bool haszero(uint32_t v) {
    constexpr uint32_t MASK = 0x7F7F7F7FU;
    return ~(((v & MASK) + MASK) | v) | MASK;
  }
  static bool notzero(uint64_t v) {
    constexpr uint64_t MASK = 0x7F7F7F7F7F7F7F7FULL;
    return ~(((v & MASK) + MASK) | v) | MASK;
  }
  uint32_t fasthash1(const char s[]) const {
    uint64_t* p = (uint64_t*)s;
    uint64_t v;
    uint64_t sum = 0xF39A5EB6;

    // https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
    while (v = *p++, hasNoZero(v)) {
      // rotate might be better, preserve more bits in each operation
#if 0
			sum = sum ^ v;
			//			sum = (sum << 17) ^ (sum << 27) ^ (sum >> 13) ^ (sum >> 24) ^ (sum >> 31) ^ sum >> 43;
			sum = (sum << 3) ^ (sum >> 31);
			sum = (sum << 7) ^ (sum >> 13);
			sum = (sum >> 10) ^ (sum << 17);
#endif
      sum = sum ^ v ^ (v << 12);
      sum = (sum << 3) ^ (sum >> 5);
      sum = (sum << 7) ^ (sum >> 13);
      sum = (sum >> 17) ^ (sum << 23);
    }
    // do the last chunk which is somewhere less than 8 bytes
    // this is for little-endian CPUs like intel, from bottom byte to the right
    uint64_t wipe = 0xFF;
    uint64_t M = 0xFFULL;
    for (int i = 8; i > 0; i--) {
      if ((v & M) == 0) {
        v &= wipe;
        break;
      }
      M <<= 8;
      wipe = (wipe << 8) | 0xFF;
    }
#if 0
		sum = sum ^ v ^ (v << 7);
		sum = (sum << 17) ^ (sum << 27) ^ (sum >> 13) ^ (sum >> 24)
			^ (sum >> 31) ^ (sum >> 45);
#endif
#if 0
		sum = sum ^ v;
		sum = (sum << 17) ^ (sum >> 13) ^ (sum >> 24)
			^ (sum >> 31) ^ (sum >> 45);
#endif
    sum = sum ^ v ^ (sum >> 3);
    sum = (sum >> 7) ^ (sum << 9);
    sum = (sum >> 13) ^ (sum << 17);
    sum = (sum >> 31) ^ (sum >> 45);
    return sum & size;
  }
  uint32_t hash(const char s[]) const {
    //		return bytewisehash(s);
    return fasthash1(s);
  }

 public:
  HashMap(uint32_t sz, uint32_t symbolSize = 1024 * 1024)
      : size(sz), table(new Node*[size]) {
    size--;
    symbols = new char[symbolSize];
    current = symbols;
    for (uint32_t i = 0; i <= size; i++) table[i] = nullptr;
  }
  ~HashMap() {
    for (uint32_t i = 0; i <= size; i++)
      for (Node* p = table[i]; p != nullptr;) {
        Node* q = p;
        p = p->next;
        delete q;
      }
    delete[] symbols;
    delete[] table;
  }
  HashMap(const HashMap& orig) = delete;
  HashMap& operator=(const HashMap& orig) = delete;
  void add(const char s[], Val v) {
    uint32_t index = hash(s);
    //		if (index = 1126) {
    //			cout << "test";
    //		}
    for (Node* p = table[index]; p != nullptr; p = p->next) {
      const char* w = symbols + p->offset;
      for (int i = 0; *w == s[i]; i++)
        if (*w == '\0') {
          p->val = v;
          return;
        }
    }
    int i;
    for (i = 0; s[i] != '\0'; i++) current[i] = s[i];
    current[i] = '\0';
    table[index] = new Node(current - symbols, v, table[index]);
    current += i + 1;
  }
  bool get(const char s[], Val& v) {
    uint32_t index = hash(s);
    for (Node* p = table[index]; p != nullptr; p = p->next) {
      const char* w = symbols + p->offset;
      for (int i = 0; *w == s[i]; w++, i++)
        if (*w == '\0') {
          v = p->val;
          return true;
        }
    }
    return false;
  }

  Val* get(const char s[]) {
    uint32_t index = hash(s);
    for (Node* p = table[index]; p != nullptr; p = p->next) {
      const char* w = symbols + p->offset;
      for (int i = 0; *w == s[i]; w++, i++)
        if (*w == '\0') {
          return &p->val;
        }
    }
    return nullptr;
  }

  void hist() {
    constexpr int histsize = 20;
    int h[histsize] = {0};
    for (uint32_t i = 0; i <= size; i++) {
      uint32_t count = 0;
      for (Node* p = table[i]; p != nullptr; p = p->next) count++;
      if (count >= size - 1)
        h[histsize - 1]++;
      else
        h[count]++;
    }
    for (int i = 0; i < histsize; i++) cout << i << '\t' << h[i] << '\n';
  }
  friend ostream& operator<<(ostream& s, const HashMap& h) {
    for (size_t i = 0; i <= h.size; i++) {
      s << "bin " << i << "\n";
      for (Node* p = h.table[i]; p != nullptr; p = p->next)
        s << h.symbols + p->offset << '\t';
      s << '\n';
    }
    return s;
  }
};

void buildHashMap2(int n) {
  HashMap<int> m(n * 2, n * 10);
  char buf[20];
  for (int i = 0; i < n; i++) {
    buf[0] = 'a' + i % 26;
    buf[1] = 'a' + i / 26 % 26;
    buf[2] = 'a' + i / (26 * 26) % 26;
    sprintf(buf + 3, "%d", i);
    m.add(buf, i);
  }
  m.hist();
}

void lookupHashMap(int n) {
  unordered_map<string, int> m;
  const char* sym[] = {"html",
                       "div",
                       "p",
                       "table",
                       "th",
                       "td",
                       "tr",
                       "form",
                       "button",
                       "drawLine",
                       "fieldpassword",
                       "fieldtext",
                       "drawRect",
                       "drawSpline",
                       "drawCircle",
                       "drawEllipse"};
  for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++) m[sym[i]] = i;

  uint32_t sum = 0;
  uint32_t c = 0;
  for (int i = 0; i < n; i++) {
    sum += m[sym[c]];
    c = (c + 1) % 16;
  }
  cout << sum << '\n';
}

void lookupHashMap2(int n) {
  HashMap<int> m(64, 1024);
  const char* sym[] = {"html",
                       "div",
                       "p",
                       "table",
                       "th",
                       "td",
                       "tr",
                       "form",
                       "button",
                       "drawLine",
                       "fieldpassword",
                       "fieldtext",
                       "drawRect",
                       "drawSpline",
                       "drawCircle",
                       "drawEllipse"};
  for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++) m.add(sym[i], i);

  uint32_t sum = 0;
  uint32_t c = 0;
  int v = 0;
  for (int i = 0; i < n; i++) {
    m.get(sym[c], v);
    sum += v;
    c = (c + 1) % 16;
  }
  cout << sum << '\n';
}

void lookupHashMap3(int n) {
  HashMap<int> m(64, 1024);
  const char* sym[] = {"html",
                       "div",
                       "p",
                       "table",
                       "th",
                       "td",
                       "tr",
                       "form",
                       "button",
                       "drawLine",
                       "fieldpassword",
                       "fieldtext",
                       "drawRect",
                       "drawSpline",
                       "drawCircle",
                       "drawEllipse"};
  for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++) m.add(sym[i], i);

  uint32_t sum = 0;
  uint32_t c = 0;
  for (int i = 0; i < n; i++) {
    int* v = m.get(sym[c]);
    sum += *v;
    c = (c + 1) % 16;
  }
  cout << sum << '\n';
}

void testCorrectness() {
  HashMap<int> m(64, 1024);
  const char* sym[] = {"html",
                       "div",
                       "p",
                       "table",
                       "th",
                       "td",
                       "tr",
                       "form",
                       "button",
                       "drawLine",
                       "fieldpassword",
                       "fieldtext",
                       "drawRect",
                       "drawSpline",
                       "drawCircle",
                       "drawEllipse"};
  for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++) m.add(sym[i], i);
  cout << m;

  for (int i = 0; i < 10; i++) {
    int* v = m.get(sym[0]);
    ++*v;
  }
  cout << m;
  m.hist();
}

void testInsertionTortureTest() {
  const int n = 1024 * 1024 * 10;
  HashMap<int> m(2 * n, n * 10);
  char buf[20];
  for (int i = 0; i < n; i++) {
    buf[0] = 'a' + i % 26;
    buf[1] = 'a' + i / 26 % 26;
    buf[2] = 'a' + i / (26 * 26) % 26;
    sprintf(buf + 3, "%d", i);
    m.add(buf, i);
    //		if (i > 10485758)
    //			cout << i << '\n';
  }
  m.hist();
}

void testInsertionBigNames(int n) {
  unordered_map<string, int> m;
  char buf[100] = "ridiculouslylongsymbolnamealwaysthesamewhocares";
  for (int i = 0; i < n; i++) {
    buf[47] = 'a' + i % 26;
    buf[48] = 'a' + i / 26 % 26;
    buf[49] = 'a' + i / (26 * 26) % 26;
    sprintf(buf + 50, "%d", i);
    m[buf] = i;
  }
}

void testInsertionBigNames2(int n) {
  HashMap<int> m(8 * n, n * 60);
  char buf[100] = "ridiculouslylongsymbolnamealwaysthesamewhocares";
  for (int i = 0; i < n; i++) {
    buf[47] = 'a' + i % 26;
    buf[48] = 'a' + i / 26 % 26;
    buf[49] = 'a' + i / (26 * 26) % 26;
    sprintf(buf + 50, "%d", i);
    m.add(buf, i);
  }
  m.hist();
}

// torture test, these symbols are all quite similar, so a great hash is
// essential
void compareInsertion() {
  constexpr int n = 1024 * 1024 * 10;
  //	bench(buildHashMap, n);
  bench(buildHashMap2, n);
}

void compareRetrieval() {
  constexpr int n = 1024 * 1024 * 100;
  bench(lookupHashMap, n);
  bench(lookupHashMap2, n);
  bench(lookupHashMap3, n);
}

void compareInsertionBigNames() {
  const int n = 1024 * 1024 * 10;
  //	bench(testInsertionBigNames, n);
  bench(testInsertionBigNames2, n);
}

int main() {
  //	testInsertionTortureTest();
  //	testInsertionBigNames();
  compareInsertionBigNames();
  // testCorrectness();
  compareInsertion();
  //	compareRetrieval();
}
