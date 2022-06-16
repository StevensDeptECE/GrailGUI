#pragma once
#include <iostream>
#include <utility>
#include <sys/stat.h>
#include "data/BlockLoader2.hh"

template<typename Val>
class BLHashMap : public BlockLoader {
 private:
  struct HashMapHeader {
    uint32_t symbolCapacity; // number of elements holding symbols
    uint32_t tableCapacity;  // number of elements holding table
    uint32_t nodeCapacity;   // number of elements holding nodes
    uint32_t nodeCount;      // number of nodes
    uint32_t symbolCount;    // number of bytes used by symbols in block of memory
    // now HashMapHeader = 24 bytes (with 4 bytes padding)
    HashMapHeader(uint32_t symbolCapacity, uint32_t tableCapacity, uint32_t nodeCapacity, uint32_t nodeCount)
     : symbolCapacity(symbolCapacity), tableCapacity(tableCapacity), nodeCapacity(nodeCapacity), nodeCount(nodeCount) {}
  };
  
  struct Node {
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
  HashMapHeader* hashMapHeader;

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
    return sum & tableCapacity; // tableCapacity MUST BE power of 2 - 1!!! No EXCEPTIONS!!
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

  struct hashReturn {
    uint32_t index;
    const char* next;
  };
  hashReturn hashAndReturn(const char s[]) const {
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
    return {sum & tableCapacity, s + i + 1};
  }

  static uint32_t getFileSize(const char filename[]) {
      struct stat s;
      stat(filename, &s);
      return s.st_size;
  }

 // setting up for constructors to avoid replicating code
  void constructorSetup(uint32_t symbolCapacity) {
    symbols = (char*)hashMapHeader + sizeof(HashMapHeader);
    nodes = (Node*)(findNextAlign8(symbols + symbolCapacity));
    table = (uint32_t*)(findNextAlign8((char*)nodes + hashMapHeader->nodeCapacity*sizeof(Node)));
    currentSymbol = symbols;
    nodeCount = 1; // zero is null

    for (uint32_t i = 0; i <= tableCapacity; i++)
      table[i] = 0;  // 0 means empty, at the moment the first node is unused
  }

  public:
  // making it the first time
  BLHashMap(uint32_t symbolCapacity, uint32_t nodeCapacity, uint32_t tableCapacity)
     : BlockLoader(symbolCapacity + nearestPower2(tableCapacity)*sizeof(uint32_t) + nodeCapacity*sizeof(Node) + sizeof(HashMapHeader), Type::namemap, 0x0002)
  {
    hashMapHeader = (HashMapHeader*)((char*)(mem) + sizeof(GeneralHeader));
    hashMapHeader->symbolCapacity = symbolCapacity;
    hashMapHeader->nodeCapacity = nodeCapacity;
    hashMapHeader->nodeCount = 0;
    hashMapHeader->tableCapacity = nearestPower2(tableCapacity)-1; // power of 2 -1 so we can AND to keep it in range [0,n-1]
    this->symbolCapacity = hashMapHeader->symbolCapacity;
    this->nodeCapacity = hashMapHeader->nodeCapacity;
    this->tableCapacity = hashMapHeader->tableCapacity;
    constructorSetup(symbolCapacity);
  }

  // loading in from blockloader file
  #if 0
  // TODO: why does this report to be < 1ms?
  // reading in blockloader file
  BLHashMap(const char filename[]) 
    : BlockLoader(filename, getFileSize(filename), getFileSize(filename)*5)
  {
    hashMapHeader = (HashMapHeader*)((char*)(mem) + sizeof(GeneralHeader));
    symbolCapacity = hashMapHeader->symbolCapacity;
    nodeCapacity = hashMapHeader->nodeCapacity;
    tableCapacity = hashMapHeader->tableCapacity;
    constructorSetup(symbolCapacity);
    uint32_t count = 0;
    char* word = strtok(symbols, "\n");
    // create hashmap and load dictionary in with a unique integer for each word
    while (word != nullptr)
    {
      add(word, count++);
      //cout << word << ": " << count << '\n';
      word = strtok(nullptr, "\n");
    }
  }
  #endif

  #if 1 // SLOWER THAN strtok()
  // reading in blockloader file
  BLHashMap(const char filename[]) 
    : BlockLoader(filename, getFileSize(filename), getFileSize(filename)*5)
  {
    hashMapHeader = (HashMapHeader*)((char*)(mem) + sizeof(GeneralHeader));
    symbolCapacity = hashMapHeader->symbolCapacity;
    nodeCapacity = hashMapHeader->nodeCapacity;
    tableCapacity = hashMapHeader->tableCapacity;
    constructorSetup(symbolCapacity);
    const char* word = symbols;
    while (word < symbols + hashMapHeader->symbolCount)
    {
      word = addAndAdvance(word);
    }
  }
  #endif

  const char* getWords() const { return symbols; }
  uint32_t getWordsSize() const { return currentSymbol - symbols; }
  const char* getNameAt(uint32_t i) const { return symbols + nodes[i].offset; }
  const Val* getValueAt(uint32_t i) const { return &nodes[i].val;}
  uint32_t getNodeCount() const { return nodeCount; }

  void writeFile(const char filename[]) {
    hashMapHeader->symbolCount = currentSymbol - symbols;
    BlockLoader::writeFile(filename, sizeof(GeneralHeader) + sizeof(HashMapHeader) + symbolCapacity + nodeCapacity*sizeof(Node));
  }

  #if 0
  ~BLHashMap() {
    delete[] nodes;
    delete[] symbols;
    delete[] table;
  }
  #endif
  BLHashMap(const BLHashMap& orig) = delete;
  BLHashMap& operator=(const BLHashMap& orig) = delete;

  // TODO: segment faults when trying to grow
  void checkGrow() {
    if (nodeCount * 2 <= tableCapacity) return;
    const Node* old = nodes;
    nodes = new Node[nodeCapacity * 2];  // TODO: need placement new
    for (uint32_t i = 0; i < nodeCapacity; i++)
      nodes[i] = std::move(old[i]);  // TODO: this is broken for objects Val
                                     // without default constructor
    nodeCapacity *= 2;
    delete[](char*) old;  // get rid of the old block of memory
    uint32_t* oldTable = table;
    uint32_t oldSize = tableCapacity;
    table = new uint32_t[tableCapacity * 2 | 1];  // new size = power of 2 - 1

    // remember size=last element, go up to AND INCLUDING
    // go through each element of existing table and copy to new one
    for (uint32_t i = 0; i <= tableCapacity; i++)
      if (oldTable[i] != 0) {
        uint32_t index = hash(
            symbols +
            nodes[oldTable[i]].offset);  // find out new hash value of symbol
        table[index] = oldTable[i];
      }
    delete[] oldTable;
    size = size * 2 | 1;
    // TODO: grow the symbol table too
    std::cerr << "BLHashMap growing size=" << tableCapacity << " " << nodeCapacity << '\n';
  }

  void add(const char s[], const Val& v) {
    uint32_t index = hash(s);
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; *w == s[i]; i++)
        if (*w == '\0') {
          nodes[p].val = v;
          return;
        }
    }
    checkGrow();

    int i;
    for (i = 0; s[i] != '\0'; i++) currentSymbol[i] = s[i];
    currentSymbol[i] = '\0';
    nodes[nodeCount] = Node(currentSymbol - symbols, table[index], v);
    table[index] = nodeCount;
    currentSymbol += i + 1;
    nodeCount++;
  }

  uint32_t getSymbolSize() {
    return currentSymbol - symbols;
  }

  // THIS FUNCTION SHOULD ONLY BE CALLED IF WE CAN GUARANTEE A UNIQUE DICTIONARY
  const char* addAndAdvance(const char s[]) {
    hashReturn r = hashAndReturn(s);
    uint32_t index = r.index;
    #if 0 // if we can guarantee uniqueness, no longer need to check
    for (uint32_t p = table[index]; p != 0; p = nodes[p].next) {
      const char* w = symbols + nodes[p].offset;
      for (int i = 0; *w == s[i]; i++)
        if (*w == '\0') {
          nodes[p].val = v;
          return r.next;
        }
    }
    #endif
    checkGrow();

    #if 0
    int i;
    for (i = 0; s[i] != '\0'; i++) currentSymbol[i] = s[i];
    currentSymbol[i] = '\0';
    currentSymbol += i + 1;
    #endif
//    nodes[nodeCount] = Node(currentSymbol - symbols, table[index], v);
    table[index] = nodeCount;
    currentSymbol = (char*)r.next;
    nodeCount++;
    return r.next;
  }

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
    currentSymbol[i] = '\0';
    nodes[nodeCount] = Node(currentSymbol - symbols, table[index], v);
    table[index] = nodeCount;
    currentSymbol += i + 1;
    nodeCount++;
    return v;
  }

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
  friend std::ostream& operator<<(std::ostream& s, const BLHashMap& h) {
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
    BLHashMap* m;
    uint32_t current;

   public:
    Iterator(const BLHashMap& list) : m(&list), current(1) {}
    bool operator!() const { return current < m->nodeCount; }
    void operator++() { ++current; }
    const char* key() const { return m->symbols[m->nodes[current].offset]; }
    Val* value() { return m->nodes[current].val; }
  };
  class ConstIterator {
   private:
    const BLHashMap* m;
    uint32_t current;

   public:
    ConstIterator(const BLHashMap& list) : m(&list), current(1) {}
    bool operator!() const { return current < m->nodeCount; }
    void operator++() { ++current; }
    const char* key() const { return m->symbols + m->nodes[current].offset; }
    const Val* value() const { return &m->nodes[current].val; }
  };
  friend Iterator;
  friend ConstIterator;
};
