#include "data/BlockMapLoader.hh"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

#include "fmt/core.h"
#include "util/Ex.hh"
#include "util/PlatFlags.hh"

using namespace std;

BlockMapLoader BlockMapLoader::loadCompressed(const char lzmaFile[]) {
  throw(Ex1(Errcode::UNIMPLEMENTED));
}

/*
  Block Loaders are high-speed binary data. While this is extremely fast, it
  means that byte order matters. You cannot write this in on an Intel and read
  in on a big-endian machine

  float: b1 b2 b3 b4 uint32_t:   b1 b2 b3 b4   b4 b3 b2 b1 b4 b3 b2 b1
  uint64_t:   b1 b2 b3 b4 b5 b6 b7 b8 --> b8 b7 b6 b5 b4 b3 b2 b1
*/
void BlockMapLoader::save(const char filename[]) {
  int fh = open(filename, O_BINARY | O_WRONLY | O_TRUNC | O_CREAT, 0644);
  int bytesWritten = write(fh, (char*)mem, size);
  if (bytesWritten < 0) {
    throw Ex2(Errcode::FILE_READ, strerror(errno));
  }
  fmt::print("{:d} bytes written\n", bytesWritten);
  close(fh);
}

static uint32_t getFileSize(const char filename[]) {
    struct stat s;
    stat(filename, &s);
    return s.st_size;
}

BlockMapLoader::BlockMapLoader(const char filename[]) : BlockLoader(filename, getFileSize(filename)) {
  blockMapHeader =
      (BlockMapHeader*)getSpecificHeader();  //((char*)mem + getHeaderSize());
  // TODO: add NamedEntities
  regionContainers = (RegionContainer*)((char*)blockMapHeader + sizeof(BlockMapHeader));
  regions = 
      (Region*)((char*)regionContainers + blockMapHeader->numRegionContainers * sizeof(RegionContainer));
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

#if 0
//TODO: test of walking through all points, but since they are float this seems worthless and weird except to test traversal
uint64_t BlockMapLoader::sum() const {
  uint64_t sum = 0;
  uint64_t* p = (uint64_t*)points;
  for (uint32_t i = blockMapHeader->numPoints / 2; i > 0; i--) sum += *p++;
  return sum;
}
#endif

// TODO: test of walking through all points, but since it's weighted towards the
// number of points it's useless as a practical measure of center anything
void BlockMapLoader::mean(float* meanx, float* meany) const {
  float xsum = 0, ysum = 0;
  const float* p = points;
  for (uint32_t i = blockMapHeader->numPoints; i > 0; i--) {
    xsum += *p++;
    ysum += *p++;
  }
  *meanx = xsum / blockMapHeader->numPoints;
  *meany = ysum / blockMapHeader->numPoints;
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
  const float* ap = a.getPoints();
  const float* bp = b.getPoints();
  for (uint32_t i = 0; i < a.getNumPoints() * 2; i++)
    if (!approxeq(ap[i], bp[i]))
      cerr << i << ": " << ap[i] << "," << bp[i] << '\n';
}
