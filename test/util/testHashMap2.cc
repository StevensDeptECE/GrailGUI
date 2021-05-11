#include <string.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace std;

void buildHashMap(int n) {
	unordered_map<string, int> m;
	char buf[20];
	for (int i = 0; i < n; i++) {
		buf[0] = 'a' + i % 26;
		buf[1] = 'a' + i / 26 % 26;
		buf[2] = 'a' + i / (26*26) % 26;
	  sprintf(buf+3, "%d", i);
		m[buf] = i;
	}
}

void benchmark(const char msg[], void (*f)(int n), int n) {
	clock_t t0 = clock();
	f(n);
	clock_t t1 = clock();
	cout << msg << " " << (t1-t0) * 1e-6 << '\n';
}

#define bench(f, n) benchmark(#f, f, n)

/*
	This version of hashmap uses relative pointers not only for the strings
	but for the node pointers themselves. Since the table is all node pointers, and
	many of them, this can be big.
 */
template<typename Val>
class HashMap {
private:
	char* symbols;
	char* current;

	class Node {
	public:
		uint32_t offset;
		uint32_t next; // relative pointer (offset into nodes)
		Val val;
		Node() {} // this is so the empty block can be initialized without doing anything
		Node(uint32_t offset, uint32_t next, Val v) : offset(offset), next(next), val(v)  {}
	};
	uint32_t nodeSize; // how many nodes are preallocated
	uint32_t currentNode; // how many nodes are currently used
	Node* nodes;
	uint32_t size;
	uint32_t* table;
	const int r1, r2, r3, r4, r5, r6; // rotate values
	
	uint32_t bytewisehash(const char s[]) const {
		uint32_t i;
		uint32_t sum = s[0] ^ 0x56F392AC;
		for (i = 1; s[i] != '\0'; i++) {
			sum = (((sum << r1) | (sum >> (32-r1))) ^ ((sum >> r2) | (sum >> (32-r2)))) + s[i];
			sum = (((sum << r3) | (sum >> (32-r3))) ^ ((sum >> r4) | (sum >> (32-r4)))) + s[i];
		}
		sum ^= i;
		sum = (((sum << r5) | (sum >> (32-r5))) ^ ((sum << r6) | (sum >> (32-r6)))) + s[i];
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

		//https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
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
		return bytewisehash(s);
		//return fasthash1(s);
	}

public:
	HashMap(uint32_t sz, uint32_t symbolSize = 1024*1024,
					int r1=5, int r2 = 7, int r3 = 17, int r4 = 13, int r5 = 11, int r6 = 16) :
		size(sz), table(new uint32_t[size]),	nodeSize(sz/2+2), nodes(new Node[sz/2+2]),
		r1(r1), r2(r2), r3(r3), r4(r4), r5(r5), r6(r6)
	{
		size--;
		symbols = new char[symbolSize];

		current = symbols;
		currentNode = 1; // zero is null
		
		for (uint32_t i = 0; i <= size; i++)
			table[i] = 0; // 0 means empty, at the moment the first node is unused
	}
	~HashMap() {
		delete [] nodes;
		delete [] symbols;
		delete [] table;
	}
	HashMap(const HashMap& orig) = delete;
	HashMap& operator =(const HashMap& orig) = delete;
  void add(const char s[], Val v) {
		uint32_t index = hash(s);
		for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
			const char* w = symbols + nodes[p].offset;
			for (int i = 0; *w == s[i]; i++)
				if (*w == '\0') {
					nodes[p].val = v;
					return;
				}
		}
		if (currentNode >= nodeSize) {
			cerr << "grew beyond table size, wow!\n";
			exit(-1);
		}
		
		int i;
		for (i = 0; s[i] != '\0'; i++)
			current[i] = s[i];
		current[i] = '\0';
		nodes[currentNode] = Node(current-symbols, table[index], v);
		table[index] = currentNode;
		current += i+1;
		currentNode++;
	}
	bool get(const char s[], Val& v) {
		uint32_t index = hash(s);
		for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
			const char* w = symbols + nodes[p].offset;
			for (int i = 0; *w == s[i]; w++, i++)
				if (*w == '\0') {
					v = nodes[p].val;
					return true;
				}
		}
		return false;
	}

	Val* get(const char s[]) {
		uint32_t index = hash(s);
		for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
			const char* w = symbols + nodes[p].offset;
			for (int i = 0; *w == s[i]; w++, i++)
				if (*w == '\0') {
					return &nodes[p].val;
				}
		}
		return nullptr;
	}

	uint64_t hist() {
		constexpr int histsize = 20;
		int h[histsize] = {0};
		for (uint32_t i = 0; i <= size; i++) {
			uint32_t count = 0;
			for (uint32_t p = table[i]; p != 0; p = nodes[p].next)
				count++;
			if (count >= histsize-1)
				h[histsize-1]++;
			else
				h[count]++;
		}
		uint64_t totalQuality = h[1];
		for (uint64_t i = 2; i < histsize; i++)
			totalQuality += i * h[i];
		cout << "Total Quality=" << totalQuality << '\n';
		const bool verbose = false;
		if (verbose) {
			for (int i = 0; i < histsize; i++) {
				if (h[i] != 0)
					cout << i << '\t' << h[i] << '\n';
			}
		}
		return totalQuality;
	}
	friend ostream& operator <<(ostream& s, const HashMap& h) {
		for (size_t i = 0; i <= h.size; i++) {
			s << "bin " << i << "\n";
			for (uint32_t p = h.table[i]; p != 0; p = h.nodes[p].next)
				s << h.symbols + h.nodes[p].offset << '\t';
			s << '\n';
		}
		return s;
	}
	
	
};

void buildHashMap2(int n) {
	HashMap<int> m(n*2, n*15);
	char buf[20];
	for (int i = 0; i < n; i++) {
		buf[0] = 'a' + i % 26;
		buf[1] = 'a' + i / 26 % 26;
		buf[2] = 'a' + i / (26*26) % 26;
	  sprintf(buf+3, "%d", i);
		m.add(buf, i);
	}
	m.hist();
}


void lookupHashMap(int n) {
	unordered_map<string, int> m;
	const	char* sym[] = {"html", "div", "p", "table",
											 "th", "td", "tr", "form",
											 "button", "drawLine", "fieldpassword", "fieldtext",
											 "drawRect", "drawSpline", "drawCircle", "drawEllipse"};	
	for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++)
		m[sym[i]] = i;

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
	const char* sym[] = {"html", "div", "p", "table",
								 "th", "td", "tr", "form",
								 "button", "drawLine", "fieldpassword", "fieldtext",
								 "drawRect", "drawSpline", "drawCircle", "drawEllipse"};	
	for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++)
		m.add(sym[i], i);

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
	const char* sym[] = {"html", "div", "p", "table",
								 "th", "td", "tr", "form",
								 "button", "drawLine", "fieldpassword", "fieldtext",
								 "drawRect", "drawSpline", "drawCircle", "drawEllipse"};	
	for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++)
		m.add(sym[i], i);

	uint32_t sum = 0;
	uint32_t c = 0;
	for (int i = 0; i < n; i++) {
		int*v = 	m.get(sym[c]);
		sum += *v;
		c = (c + 1) % 16;
	}
	cout << sum << '\n';
}

void testBasics() {
	HashMap<int> m(64, 1024);
	const char* sym[] = {"html", "div", "p", "table",
								 "th", "td", "tr", "form",
								 "button", "drawLine", "fieldpassword", "fieldtext",
								 "drawRect", "drawSpline", "drawCircle", "drawEllipse"};	
	for (size_t i = 0; i < sizeof(sym) / sizeof(char*); i++)
		m.add(sym[i], i);
	cout << m;
	
  for (int i = 0; i < 10; i++) {
		int* v = m.get(sym[0]);
		++*v;
	}
	cout << m;
	m.hist();
}

uint64_t testInsertionTortureTest(const int n = 1024*1024*8,
																	int r1 = 2, int r2 = 3, int r3 = 5, int r4 = 7,
																	int r5 = 13, int r6 = 17) {
	const int mod = 1;
	HashMap<int> m(4*n, n * 15, r1, r2, r3, r4, r5, r6);
	char buf[20];
	for (int i = 0; i < n; i++) {
		buf[0] = 'a' + i % 26;
		buf[1] = 'a' + i / 26 % 26;
		buf[2] = 'a' + i / (26*26) % 26;
	  sprintf(buf+3, "%d", i);
		m.add(buf, i);
		//		if (i > 9633515)
		//				cout << i << '\n';
}	
	return m.hist();
}

void testInsertionBigNames(int n) {
	unordered_map<string,int> m;
	char buf[100] = "ridiculouslylongsymbolnamealwaysthesamewhocares";
	for (int i = 0; i < n; i++) {
		buf[47] = 'a' + i % 26;
		buf[48] = 'a' + i / 26 % 26;
		buf[49] = 'a' + i / (26*26) % 26;
	  sprintf(buf+50, "%d", i);
		m[buf] = i;
	}
}

void testInsertionBigNames2(int n) {
	HashMap<int> m(4*n, n*60);
	char buf[100] = "ridiculouslylongsymbolnamealwaysthesamewhocares";
	for (int i = 0; i < n; i++) {
		buf[47] = 'a' + i % 26;
		buf[48] = 'a' + i / 26 % 26;
		buf[49] = 'a' + i / (26*26) % 26;
	  sprintf(buf+50, "%d", i);
		m.add(buf, i);
	}
	m.hist();

}

void testCorrectness() {
	testBasics();
	const int n = 1024*1024*8;
	testInsertionTortureTest(n);
	//	testInsertionBigNames2(n);
}

void findOptimalRotateNumbers() {
	int rot[] = {11, 8, 13, 17, 19, 2, 10, 3, 5, 7};
	uint64_t minCost = -1LL;
	int r1, r2, r3, r4, r5, r6;
	const int n = 1024*1024*8;
	unordered_map<int,bool> visited;
	while (std::next_permutation(rot, rot + sizeof(rot)/sizeof(int))) {
		uint64_t sig = ((((rot[0]*20+rot[1])*20+rot[2])*20+rot[3])*20+rot[4])*20+rot[5];
		if (visited.find(sig) != visited.end())
			continue;
		visited[sig] = true;
		cout << "trying " << rot[0] << " " << rot[1] << " " << rot[2] <<
				" " << rot[3] << " " << rot[4] << " " << rot[5] << '\n';
		uint64_t cost =
			testInsertionTortureTest(n, rot[0], rot[1], rot[2], rot[3], rot[4], rot[5]);
		if (cost < minCost) {
			minCost = cost;
			r1 = rot[0];
			r2 = rot[1];
			r3 = rot[2];
			r4 = rot[3];
			r5 = rot[4];
			r6 = rot[5];
			cout << "best so far " << r1 << " " << r2 << " " << r3 <<
				" " << r4 << " " << r5 << " " << r6 << '\n';
		}
	}
}
//torture test, these symbols are all quite similar, so a great hash is essential
void compareInsertion() {
	constexpr int n = 1024*1024*8;
	//	bench(buildHashMap, n);
	bench(buildHashMap2, n);
}

void compareRetrieval() {
	constexpr int n = 1024*1024*64;
	bench(lookupHashMap, n);
	bench(lookupHashMap2, n);
	bench(lookupHashMap3, n);
}

void compareInsertionBigNames() {
	const int n = 1024*1024*8;
	bench(testInsertionBigNames, n);
	bench(testInsertionBigNames2, n);
}

int main() {
	const int n = 1024*1024*8;
	bench(buildHashMap2, n);
	
	findOptimalRotateNumbers();
	//	testCorrectness();
	//testInsertionBigNames();
	//compareInsertionBigNames();
	//	compareInsertion();
	//	compareRetrieval();
	//lookupHashMap(10);
}
