#include "BlockMapLoader.hh"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void BlockLoader::init(uint64_t bytes, Type t, uint32_t version) {
  bytes =
      (getHeaderSize() + bytes + 7) & ~7ULL;  // round up to 64-bit aligned size
  size = bytes;                               // store size in teh object
  // mem = std::make_unique<uint64_t>(bytes / 8);
  mem = new uint64_t[bytes / 8];
  generalHeader = (GeneralHeader*)mem;  // header is the first chunk of bytes
  generalHeader->magic = ((((('!' << 8) + 'B') << 8) + 'L') << 8) +
                         'd';  // magic number for all block loaders
  generalHeader->type = uint32_t(t);
  generalHeader->version = version;
  securityHeader =
      (SecurityHeaderV0*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
}

BlockLoader::BlockLoader(const char filename[]) {
  int fh = open(filename, O_RDONLY);
  struct stat s;
  fstat(fh, &s);
  uint64_t size = (s.st_size + 7) & ~7ULL;
  read(fh, (char*)mem, size);
  close(fh);
  generalHeader = (GeneralHeader*)mem;  // header is the first chunk of bytes
  securityHeader =
      (SecurityHeaderV0*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
}

void BlockMapLoader::save(const char filename[]) {
  int fh = open(filename, O_WRONLY | O_CREAT);
  write(fh, (char*)mem, size);
  close(fh);
  // byte-endian-ness matters! You cannot write this in on an Intel and read in
  // on Sparc

  // if you have to worry about endianness
  // float: b1 b2 b3 b4 uint32_t:   b1 b2 b3 b4   b4 b3 b2 b1 b4 b3 b2 b1
  // uint64_t:   b1 b2 b3 b4 b5 b6 b7 b8 --> b8 b7 b6 b5 b4 b3 b2 b1
}
BlockMapLoader::BlockMapLoader(const char filename[]) : BlockLoader(filename) {
  // floats are now completely loaded, ready to draw!
}
typedef void (BlockMapLoader::*Method)();
// Method BlockMapLoader::methods[] = {&methodPolygon};

/*
        TODO: For now we will just call methodPolygon
        Later we have to call the method that applies to the type.
        We will group all of the same type together so they can be
        executed in a single block.

        Perhaps teh methods should contain a start index and length to draw
 */
void BlockMapLoader::methodPolygon() {}

void BlockMapLoader::methodPolyline() {}
