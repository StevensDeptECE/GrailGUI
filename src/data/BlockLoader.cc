#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

#include "BlockMapLoader.hh"
//#include "lzmadec.h"
using namespace std;

BlockLoader::BlockLoader(const char filename[]) {
  int fh = open(filename, O_RDONLY);
  if (fh < 0) throw "Can't open file";  // TODO: Use Ex.hh to report location
  struct stat s;
  fstat(fh, &s);
  size = s.st_size;

  mem = new uint64_t[(size + 7) / 8];
  int bytesRead = read(fh, (char*)mem, size);
  if (bytesRead != size)
    throw "Could not read entire file";  // TODO: Use Ex.hh to report location
  init(mem, size);
  close(fh);
}

#if 0

/*
TODO: if compressed, a block loader will call lzmaDecompress, 
load itself into memory, decompress itself, 
then set up the single block of memory.
This will take a lot more CPU, and more time, but if the file is
large and it is being transmitted over the network, it is very worthwhile.
Note that lzma is much slower to compress, so precompressed files are
very effective but if they have to be continuously compressed, it may not
be worth it.

In the future, we can add support for other kinds of compression like bzip2.
A compressed file will require some way of identifying that it is compressed
a byte size of the uncompressed file, and the type of compression

One possible alternate header would be:
!BLc (Block Loader compressed instead of Block Loader data)
4 byte length

Either add subsidary type information for different compression or have other magic numbers
for example
!BLC (compressed using bzip2)
!BLx (compressed using lzma, but with 64 bit length?)
etc.
*/
static void *_lzmaAlloc(ISzAllocPtr, size_t size) {
  return new uint8_t[size];
}
static void _lzmaFree(ISzAllocPtr, void *addr) {
  if (!addr)
    return;
  
  delete[] reinterpret_cast<uint8_t *>(addr);
}

static ISzAlloc _allocFuncs = {
  _lzmaAlloc, _lzmaFree
};

BlockMapLoader BlockMapLoader::loadCompressed(const char lzmaFile[]) {
  int fh = open(filename, O_RDONLY);
  struct stat s;
  fstat(fh, &s);
  uint64_t compressedSize = s.st_size;
  unique_ptr<char[]> tempCompressed = make_unique(compressedSize); // create temp buffer to hold compressed data
  int bytesRead = read(fh, tempCompressed.get(), compressedSize); // read in compressed file
  if (bytesRead != compressedSize)
    throw "Error: compressed file size mismatch";
  close(fh);//TODO: make sure this file handle is automatically closed if the throw happens
  
  if (compressedSize < 13)
    throw "Compressed file too small to be correct lzma file";

  // this is lzma decompressed courtesy of
  //https://gist.github.com/phoe/c33e1f8ec651e7892f82596be6d0d3af
  
  char* input = tempCompressed.get();
  // extract the size from the header
  uint64_t size = 0;
  for (int i = 0; i < 8; i++)
    size |= (input[5 + i] << (i * 8));

  if (size <= (256 * 1024 * 1024)) {
    auto blob = std::make_unique<uint8_t[]>(size);
    ELzmaStatus lzmaStatus;
    SizeT procOutSize = size, procInSize = compressedSize - 13;
    int status = LzmaDecode(blob.get(), &procOutSize, &input[13],
			    &procInSize, input, 5, LZMA_FINISH_END, &lzmaStatus, &_allocFuncs);

    if (status == SZ_OK && procOutSize == size) {
      *outputSize = size;
      return BlockMapLoader();
    }
  }
  throw "Error: lZMA compression using more than 256Mb, giving up";
}
#endif

/*
  Save out a BlockLoader to disk in a single write.
  This should work for all children, their only responsibility is
  to fill allocated memory with the correct byte structure
*/
void BlockLoader::save(const char filename[]) const {
  int fh = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
  int bytesWritten = write(fh, (char*)mem, size);
  close(fh);
  // byte-endian-ness matters! You cannot write this in on an Intel and read in
  // on Sparc

  // if you have to worry about endianness
  // float: b1 b2 b3 b4 uint32_t:   b1 b2 b3 b4   b4 b3 b2 b1 b4 b3 b2 b1
  // uint64_t:   b1 b2 b3 b4 b5 b6 b7 b8 --> b8 b7 b6 b5 b4 b3 b2 b1
}
