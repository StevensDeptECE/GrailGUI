#pragma once

#include <memory>

#include "opengl/Shapefile.hh"

class BlockLoader {
 public:
  struct GeneralHeader {
    uint32_t type;     // type of block loader
    uint32_t version;  // version
  };
  struct SecurityHeaderV1 {
    uint8_t hash[32];  // SHA256 hash signed by author
    uint8_t
        hash2[32];  // SHA256 hash of alternate region in data signed by author
    uint8_t sigid[32];  // id of author
  };

  // get the size of the SecurityHeader for any BlockLoader
  uint32_t getAuthHeaderSize() const;
  bool authenticate() const;
};

class BlockMapLoader : public BlockLoader, public ESRIShape {
 public:
  struct SpecificHeader {
    uint32_t numLists;
    uint32_t unused;
  };
  struct Segment {
    uint32_t numPoints;
    uint32_t type : 8;
    uint32_t unused : 8;
    double baseLocX, baseLocY;
  };

 private:
  SpecificHeader* specificHeader;
  Segment* segments;
  const float* points;

 public:
  BlockMapLoader(uint32_t numLists, uint32_t numPoints) {
    uint32_t headerWords =
        ((sizeof(Header) + numLists * sizeof(Segment)) + 7) / 8;
    uint32_t size = (headerWords + numPoints) * 8;
    mem = std::make_unique<uint64_t[headerWords + numPoints]>;
    head = (Header*)mem;  // header is the first chunk of bytes
    segments =
        (Segment*)((char*)mem + sizeof(Header));  // list of segments is next
    points =
        (float*)((char*)mem + sizeof(Header) + numPoints * sizeof(Segment));

    // load in all points from ESRI

    int fh = open("uscounties.bml", O_WRONLY | O_BINARY);
    write(fh, (char*)mem, size);
    close(fh);
    // byte-endian-ness matters! You cannot write this in on an Intel and read
    // in on Sparc

    // if you have to worry about endianness
    // float: b1 b2 b3 b4 uint32_t:   b1 b2 b3 b4   b4 b3 b2 b1 b4 b3 b2 b1
    // uint64_t:   b1 b2 b3 b4 b5 b6 b7 b8 --> b8 b7 b6 b5 b4 b3 b2 b1
  }

  BlockMapLoader(const char filename[]) {
    int fh = open("uscounties.bml", O_WRONLY | O_BINARY);
    st_stat s;
    stat(fh, &s);
    mem = new uint64_t[s.st_size / 8];
    read(fh, (char*)mem, size);
    close(fh);
    // floats are now completely loaded, ready to draw!
  }
