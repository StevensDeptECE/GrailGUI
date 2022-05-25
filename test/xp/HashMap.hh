#pragma once
#include <iostream>
#include <utility>
#include "data/BlockLoader2.hh"

class HashMapBase : BlockLoader {
 protected:
  struct HashMapHeader {
    uint32_t symbolCapacity; // size of memory block holding symbols
    uint32_t tableCapacity;  // size of memory block holding table
    uint32_t nodeCapacity;   // size of memory block holding nodes
    uint32_t unused;         // make C++ align to 8 bytes (now guarantee 16 bytes)
    HashMapHeader(uint32_t symbolCapacity, uint32_t tableCapacity, uint32_t nodeCapacity)
     : symbolCapacity(symbolCapacity), tableCapacity(tableCapacity), nodeCapacity(nodeCapacity) {}
  };
  
  uint32_t symbolCapacity; // size of memory block holding symbols
  uint32_t tableCapacity;  // size of memory block holding table
  char* symbols;
  char* current;
  uint32_t* table;
  HashMapHeader* hashMapHeader;

  constexpr static int r1 = 5, r2 = 7, r3 = 17, r4 = 13, r5 = 11,
                       r6 = 16;  // rotate values

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

  uint32_t fasthash1(const char s[]) const;
  uint32_t bytewisehash(const char s[], uint32_t len) const;
  uint32_t bytewisehash(const char s[]) const;
  uint32_t hash(const char s[]) const { return bytewisehash(s); }
  uint32_t hash(const char s[], uint32_t len) const {
    return bytewisehash(s, len);
  }
  HashMapBase(uint32_t symbolCapacity, uint32_t tableCapacity)
      : tableCapacity(tableCapacity), symbolCapacity(symbolCapacity), table(new uint32_t[tableCapacity]) {
    tableCapacity--;
    symbols = new char[symbolCapacity];
  }

  HashMapBase(uint32_t symbolCapacity, uint32_t tableCapacity, uint32_t nodeCapacity, uint32_t additional)
     : BlockLoader(symbolCapacity + tableCapacity + nodeCapacity + additional, Type::namemap, 0x0001)
  {
    hashMapHeader = (HashMapHeader*)((char*)(mem) + sizeof(GeneralHeader));
    symbolCapacity = hashMapHeader->symbolCapacity;
    tableCapacity = hashMapHeader->tableCapacity;
    symbols = (char*)hashMapHeader + sizeof(HashMapHeader);
    table = (uint32_t*)(symbols + symbolCapacity);

  }

 public:
  const char* getWords() const { return symbols; }
  uint32_t getWordsSize() const { return current - symbols; }
};

template <typename Val>
class HashMap : public HashMapBase {
  struct Node {
    uint32_t offset;
    uint32_t next;  // relative pointer (offset into nodes)
    Val val;
    Node() {
    }  // this is so the empty block can be initialized without doing anything
    Node(uint32_t offset, uint32_t next, Val v)
        : offset(offset), next(next), val(v) {}
  };
  uint32_t nodeCapacity;   // how many nodes are preallocated
  uint32_t nodeCount;  // how many nodes are currently used
  Node* nodes;

 public:
  HashMap(uint32_t symbolCapacity, uint32_t tableCapacity, uint32_t nodeCapacity)
      : HashMapBase(symbolCapacity, tableCapacity, nodeCapacity*sizeof(Node), sizeof(HashMapBase))
        nodeCapacity(nodeCapacity),
        nodes(new Node[nodeCapacity]) {
    current = symbols;
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
    std::cerr << "HashMap growing size=" << tableCapacity << " " << nodeCapacity << '\n';
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
    for (i = 0; s[i] != '\0'; i++) current[i] = s[i];
    current[i] = '\0';
    nodes[nodeCount] = Node(current - symbols, table[index], v);
    table[index] = nodeCount;
    current += i + 1;
    nodeCount++;
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
    for (i = 0; i < len; i++) current[i] = s[i];
    current[i] = '\0';
    nodes[nodeCount] = Node(current - symbols, table[index], v);
    table[index] = nodeCount;
    current += i + 1;
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
