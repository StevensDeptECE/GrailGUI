#pragma once
#include <iostream>
#include <utility>

template <typename Val>
class HashMap {
  class Node {
   public:
    uint32_t offset;
    uint32_t next;  // relative pointer (offset into nodes)
    Val val;
    Node() {
    }  // this is so the empty block can be initialized without doing anything
    Node(uint32_t offset, uint32_t next, Val v)
        : offset(offset), next(next), val(v) {}
  };

  uint32_t symbolCapacity; // number of elements holding symbols
  uint32_t nodeCapacity;   // how many nodes are preallocated
  uint32_t tableCapacity;  // number of elements  holding table
  char* symbols;
  char* currentSymbol;
  uint32_t nodeCount;  // how many nodes are currently used
  Node* nodes;
  uint32_t* table;

  constexpr static int r1 = 5, r2 = 7, r3 = 17, r4 = 13, r5 = 11,
                       r6 = 16;  // rotate values

  // computes the next highest power of 2
  static uint32_t nearestPower2 (uint32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
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
    sum = sum ^ v ^ (sum >> 3);
    sum = (sum >> 7) ^ (sum << 9);
    sum = (sum >> 13) ^ (sum << 17);
    sum = (sum >> 31) ^ (sum >> 45);
    return sum & tableCapacity;
  }

  // do not call with len = 0, or die.
  uint32_t bytewisehash(const char s[], uint32_t len) const {
    uint32_t i;
    uint32_t sum = s[0] ^ 0x56F392AC;
    for (i = 1; i < len; i++) {
      sum = (((sum << r1) | (sum >> (32 - r1))) ^
            ((sum >> r2) | (sum >> (32 - r2)))) +
            s[i];
      sum = (((sum << r3) | (sum >> (32 - r3))) ^
            ((sum >> r4) | (sum >> (32 - r4)))) +
            s[i];
    }
    sum = ((sum << r5) | (sum >> (32 - r5))) ^ sum ^ (i << 7);
    //  sum = (((sum << r5) | (sum >> (32-r5))) ^ ((sum << r6) | (sum >>
    //  (32-r6)))) + s[i];
    return sum & tableCapacity;
  }

  uint32_t bytewisehash(const char s[]) const {
    uint32_t i;
    uint32_t sum = s[0] ^ 0x56F392AC;
    for (i = 1; s[i] != '\0'; i++) {
      sum = (((sum << r1) | (sum >> (32 - r1))) ^
            ((sum >> r2) | (sum >> (32 - r2)))) +
            s[i];
      sum = (((sum << r3) | (sum >> (32 - r3))) ^
            ((sum >> r4) | (sum >> (32 - r4)))) +
            s[i];
    }
    sum = ((sum << r5) | (sum >> (32 - r5))) ^ sum ^ (i << 7);
    //  sum = (((sum << r5) | (sum >> (32-r5))) ^ ((sum << r6) | (sum >>
    //  (32-r6)))) + s[i];
    return sum & tableCapacity;
  }

  uint32_t hash(const char s[]) const { return bytewisehash(s); }
  uint32_t hash(const char s[], uint32_t len) const {
    return bytewisehash(s, len);
  }

  void checkGrow() {
    if (nodeCount * 2 <= tableCapacity) return;
    const Node* oldNodes = nodes;
    nodes = new Node[nodeCapacity * 2];  // TODO: need placement new
    for (uint32_t i = 0; i < nodeCount; i++)
      nodes[i] = std::move(oldNodes[i]);  // TODO: this is broken for objects Val
                                     // without default constructor
    nodeCapacity *= 2;
    delete[](char*) oldNodes;  // get rid of the old block of memory
    uint32_t* oldTable = table;
    uint32_t oldTableCapacity = tableCapacity;
    tableCapacity = tableCapacity*2|1;  // this is power of 2 - 1
    table = new uint32_t[tableCapacity+1];  // new size = power of 2
    for (uint32_t i = 0; i <= tableCapacity; i++)
      table[i] = 0;
    // remember tableCapacity=last element, go up to AND INCLUDING
    // go through each element of existing table and copy to new one
    for (uint32_t i = 0; i <= oldTableCapacity; i++)
      if (oldTable[i] != 0) {
        // find out new hash value of symbol
        uint32_t index = hash(symbols + nodes[oldTable[i]].offset); 
        if (table[index] != 0) {
          nodes[i].next = table[index]; // make our node point at the one already in the bin
        }          
        table[index] = oldTable[i];
      }
    delete[] oldTable;
    // TODO: grow the symbol table too
    std::cerr << "HashMap growing size=" << tableCapacity << " " << nodeCapacity << '\n';
  }

  void internalUpdate(int i, uint32_t index, const Val& v) {
    currentSymbol[i] = '\0';
    nodes[nodeCount] = Node(currentSymbol - symbols, table[index], v);
    table[index] = nodeCount;
    currentSymbol += i + 1;
    nodeCount++;
  }

  void internalAdd(uint32_t index, const char s[], const Val& v) {
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; w[i] == s[i]; i++)
        if (w[i] == '\0') {
          nodes[p].val = v;
          return;
        }
    }
    int i;
    for (i = 0; s[i] != '\0'; i++) currentSymbol[i] = s[i];
    internalUpdate(i, index, v);
  }

  
  //TODO: This logic is NOT RIGHT.
  // for case where "catalog"--> bin 12, then "cat" --> bin 12, this would think
  // that cat should replace catalog since the first 3 letters of catalog match cat.
  // 
  void internalAdd(uint32_t index, const char s[], uint32_t len, const Val& v) {
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; i < len; i++) {
        if (w[i] != s[i]) {
          goto notAMatch; // if a letter is different, this is the wrong hash entry
        }
      }
      if (w[len] == '\0') {
        nodes[p].val = v;
        return;
      }
    notAMatch: ;
    }
    int i;
    for (i = 0; i < len; i++) currentSymbol[i] = s[i];
    internalUpdate(i, index, v);
  }
 public:
  HashMap(uint32_t tableCapacity_in, uint32_t symbolCapacity = 1024 * 1024)
      : symbolCapacity(symbolCapacity),
        tableCapacity(tableCapacity_in - 1) {
    // allocate a power of 2 for the table size, and set tableCapacity to n-1 so it can AND to implement MOD
    nodeCapacity = tableCapacity / 2+2;
    symbols = new char[symbolCapacity];
    table = new uint32_t[tableCapacity+1];
    nodes = new Node[tableCapacity / 2+2];
    currentSymbol = symbols;
    nodeCount = 1;  // zero is null

    for (uint32_t i = 0; i <= tableCapacity; i++)
      table[i] = 0;  // 0 means empty, at the moment the first node is unused
  }
  ~HashMap() {
    delete[] nodes;
    delete[] symbols;
    delete[] table;
  }
  HashMap(const HashMap& orig) = delete;
  HashMap& operator=(const HashMap& orig) = delete;

  void add(const char s[], uint32_t len, const Val& v) {
    checkGrow();
    uint32_t index = hash(s, len);
    internalAdd(index, s, len, v);
  }

  void add(const char s[], const Val& v) {
    checkGrow();
    uint32_t index = hash(s);
    internalAdd(index, s, v);
  }

  
#if 0
  Val add(const char s[], uint32_t len, const Val& v) {
    uint32_t index = hash(s, len);
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; *w == s[i] && i < len; i++)
        if (*w == '\0') {
          throw "Error, this should not happen";
        }
    }
    checkGrow();

    int i;
    for (i = 0; i < len; i++) currentSymbol[i] = s[i];
    current[i] = '\0';
    nodes[nodeCount] = Node(currentSymbol - symbols, table[index], v);
    table[index] = nodeCount;
    currentSymbol += i + 1;
    nodeCount++;
    return v;
  }
#endif

  bool get(const char s[], Val* v) const {
    uint32_t index = hash(s);
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; *w == s[i]; w++, i++)
        if (*w == '\0') {
          *v = nodes[p].val;
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

  const Val* get(const char s[]) const {
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

  Val* get(const char* s, uint32_t len) {
    uint32_t index = hash(s);
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; *w == s[i]; w++, i++)
        if (i == len) return &nodes[p].val;
    }
    return nullptr;
  }

  const char* getWords() const { return symbols; }
  uint32_t getWordsSize() const { return currentSymbol - symbols; }

  uint64_t hist() const {
    constexpr int histsize = 20;
    int h[histsize] = {0};
    for (uint32_t i = 0; i <= tableCapacity; i++) {
      uint32_t count = 0;
      for (uint32_t p = table[i]; p != 0; p = nodes[p].next) count++;
      if (count >= histsize - 1)
        h[histsize - 1]++;
      else
        h[count]++;
    }
    uint64_t totalQuality = h[1];
    for (uint64_t i = 2; i < histsize; i++) totalQuality += i * h[i];
    std::cout << "Total Quality=" << totalQuality << '\n';
    const bool verbose = true;
    if (verbose) {
      for (int i = 0; i < histsize; i++) {
        if (h[i] != 0) std::cout << i << '\t' << h[i] << '\n';
      }
    }
    return totalQuality;
  }
  friend std::ostream& operator<<(std::ostream& s, const HashMap& h) {
    for (size_t i = 0; i <= h.size; i++) {
      s << "bin " << i << "\n";
      for (uint32_t p = h.table[i]; p != 0; p = h.nodes[p].next)
        s << h.symbols + h.nodes[p].offset << '\t';
      s << '\n';
    }
    return s;
  }

  class Iterator {
   private:
    HashMap* m;
    uint32_t current;

   public:
    Iterator(const HashMap& list) : m(&list), current(1) {}
    bool operator!() const { return current < m->nodeCount; }
    void operator++() { ++current; }
    const char* key() const { return m->symbols[m->nodes[current].offset]; }
    Val* value() { return m->nodes[current].val; }
  };
  class ConstIterator {
   private:
    const HashMap* m;
    uint32_t current;

   public:
    ConstIterator(const HashMap& list) : m(&list), current(1) {}
    bool operator!() const { return current < m->nodeCount; }
    void operator++() { ++current; }
    const char* key() const { return m->symbols + m->nodes[current].offset; }
    const Val* value() const { return &m->nodes[current].val; }
  };
  friend Iterator;
  friend ConstIterator;
};
