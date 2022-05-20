#pragma once

#include "DynArray.hh"
template <uint32_t blockSize>
class BlockAllocator {
 private:
  class Block {
   public:
    uint64_t* p;      // pointer to the block of memory
    uint32_t offset;  // offset to the current memory
    Block(char* p, uint32_t size) : p(p), offset(0) {}
    ~Block() { delete[] p; }
    Block(const Block& orig) = delete;
    Block& operator=(const Block& orig) = delete;
    Block(Block&& orig) : p(orig.p), offset(orig.offset) { orig.p = nullptr; }
  };
  DynArray<Block> blocks;
  uint64_t* p;       // pointer to the block of memory
  uint32_t current;  // offset to the current memory
 public:
  BlockAllocator(uint32_t size) : blocks(size) {
    p = new char[blockSize];
    current = 0;
  }
  ~BlockAllocator() {
    delete[] p;  // delete the current pointer
  }              // destructor for DynArray will wipe all the old ones

  void* alloc(uint32_t size) {
    if (blockSize - current < size) {
      blocks.add(Block(p, size));
      if (size > blockSize) {
        p = new char[size];
        current = 0;
      }
    }
    char* temp = p + current;
    current += size;
    return temp;
  }
};

void* operator new(size_t sz, BlockAllocator* b) { return b->alloc(sz); }
