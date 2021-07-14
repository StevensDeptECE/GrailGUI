#include "BlockMapLoader.hh"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
//#include "lzmadec.h"
using namespace std;

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

void BlockLoader::init(uint64_t* mem, uint64_t size) {
  this->mem = mem;
  this->size = size;
  this->generalHeader = (GeneralHeader*)mem;
  this->securityHeader =
      (SecurityHeaderV0*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
}
#if 0
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

void BlockMapLoader::save(const char filename[]) {
  int fh = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
  int bytesWritten = write(fh, (char*)mem, size);
  close(fh);
  // byte-endian-ness matters! You cannot write this in on an Intel and read in
  // on Sparc

  // if you have to worry about endianness
  // float: b1 b2 b3 b4 uint32_t:   b1 b2 b3 b4   b4 b3 b2 b1 b4 b3 b2 b1
  // uint64_t:   b1 b2 b3 b4 b5 b6 b7 b8 --> b8 b7 b6 b5 b4 b3 b2 b1
}
BlockMapLoader::BlockMapLoader(const char filename[]) : BlockLoader(filename) {
  blockMapHeader = (BlockMapHeader*)((char*)mem + getHeaderSize());
  // TODO: add RegionContainer and NamedEntities
  regions = (Region*)((char*)blockMapHeader + sizeof(BlockMapHeader));
  segments =
      (Segment*)((char*)regions + blockMapHeader->numRegions * sizeof(Region));
  points =
      (float*)((char*)segments + blockMapHeader->numSegments * sizeof(Segment));

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

uint64_t BlockMapLoader::sum() const {
  uint64_t sum = 0;
  uint64_t* p = (uint64_t*)points;
  for (uint32_t i = blockMapHeader->numPoints / 2; i > 0; i--) sum += *p++;
  return sum;
}

inline void delta(float& val, float& prevVal) {
  float temp = val;
  val -= prevVal;
  prevVal = temp;
}

void BlockMapLoader::deltaEncode() {
  uint32_t numSegments = blockMapHeader->numSegments;
  float* xPoints = points;
  float* yPoints = points + blockMapHeader->numPoints;

  for (uint32_t i = 0; i < numSegments; i++) {
    const uint32_t numPoints = segments[i].numPoints;
    float lastX = regions[i].baseX;
    float lastY = regions[i].baseY;
    for (uint32_t j = 0; j < numPoints; j++) {
      delta(xPoints[j], lastX);
      delta(yPoints[j], lastY);
    }
  }
}

void BlockMapLoader::deltaUnEncode() {
  uint32_t numSegments = blockMapHeader->numSegments;
  float* xPoints = points;
  float* yPoints = points + blockMapHeader->numPoints;
  for (uint32_t i = 0; i < numSegments; i++) {
    const uint32_t numPoints = segments[i].numPoints;
    double lastX = regions[i].baseX;
    double lastY = regions[i].baseY;
    for (uint32_t j = 0; j < numPoints; j++) {
      lastX += xPoints[i], lastY += yPoints[i];
      xPoints[i] = lastX, yPoints[i] = lastY;
    }
  }
}

void BlockMapLoader::dumpSegment(uint32_t seg) {
  uint32_t numSegments = blockMapHeader->numSegments;
  if (seg >= numSegments) return;
  const float* xy = points;

  uint32_t pointIndex = 0;
  for (uint32_t i = 0; i < seg; i++) {
    pointIndex += segments[i].numPoints;
  }
  pointIndex *= 2;  // xy points together, so double the number
  const uint32_t numPoints = segments[seg].numPoints;
  for (uint32_t j = 0; j < numPoints; j++, pointIndex += 2) {
    printf("%14.7lf%14.7lf\n", xy[pointIndex], xy[pointIndex + 1]);
  }
}

void BlockMapLoader::diff(const BlockMapLoader& a, const BlockMapLoader& b) {
  const BlockMapHeader* ahead = a.getBlockMapHeader();
  const BlockMapHeader* bhead = b.getBlockMapHeader();
  uint32_t errors = 0;
  // if (!approxeq(ahead->bounds, bhead->bounds))
  //  cout << ahead->bounds << bhead->bounds;
  if (ahead->numRegions != bhead->numRegions) {
    cerr << "diff: number of regions " << ahead->numRegions << " "
         << bhead->numRegions << '\n';
    errors++;
  } else {
    const Region* areg = a.getRegions();
    const Region* breg = b.getRegions();
  }
  if (ahead->numSegments != bhead->numSegments) {
    cerr << "diff: number of segments " << ahead->numSegments << " "
         << bhead->numSegments << '\n';
    errors++;
  } else {
    for (uint32_t i = 0; i < ahead->numSegments; i++) {
      const Segment* aseg = a.segments + i;
      const Segment* bseg = b.segments + i;
      if (aseg->numPoints != bseg->numPoints)
        cerr << "diff: size of segment " << i << " " << aseg->numPoints << " "
             << bseg->numPoints << '\n';
      errors++;
    }
  }
  if (errors != 0) return;
  const float* ap = a.getXPoints();
  const float* bp = b.getXPoints();
  for (uint32_t i = 0; i < a.getNumPoints() * 2; i++)
    if (!approxeq(ap[i], bp[i]))
      cerr << i << ": " << ap[i] << "," << bp[i] << '\n';
}