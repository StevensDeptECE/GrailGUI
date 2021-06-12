#pragma once
#include "BlockLoader.hh"

class BlockMapLoader : public BlockLoader {
 public:
  struct BoundRect {
    float xMin, xMax, yMin, yMax;
  };

  struct BlockMapHeader {
    uint32_t numNamedEntities;
    uint32_t numRegionContainers;
    uint32_t numRegions;
    uint32_t numSegments;
    uint32_t totalPoints;
    uint32_t deltaEncoded : 1;
    BoundRect bounds;
  };

  /*
    RegionContainer represents a hierarchy where a bigger entity (like a state)
    contains a number of smaller entities (like counties)
    TODO: open question whether this should contain a list of regions or a
    start,end and require them to be sequential. The advantage is compactness
    vs. the ability to represent shared regions owned by two larger entities.

    Note that the smaller approach works for a strict hierarchy like US contains
    States and territories which contain counties.
    TODO: Implement optional headers which will contain the Regions and
    RegionContainers
  */

  enum class EntityType { REGION_CONTAINER, REGION, SEGMENT, POINT };
  struct NamedEntry {
    uint32_t offsetStart;  // starting byte of the name;
    uint8_t len;           // length of the name (limited to 256 bytes)
    uint8_t entityType;    // entity cast as 8 bit number
    uint8_t type;  // feature type of name (could be used to choose font)
  };
  struct NamedEntities {
    uint32_t numNames;  // TODO: redundant with the number in the BlockMapHeader
    uint32_t nameSizes;  // amount of memory used for all names
    NamedEntry entry[];
  };
  struct RegionContainer {
    uint32_t startRegion, endRegion;  // the range of regions
    BoundRect bounds;
  };
  /*
    A region is a local area containing a number of segments.
    It has a baselocation x,y in double, and all the segments may be relative to
    that. It has a range of segments that correspond to this area.
  */
  struct Region {
    uint32_t segmentStart;  // first segment in this region
    uint32_t segmentEnd;    // last semgment in this region
    uint32_t startPoints;   // starting index for points so you don't have to
                            // start from the beginning to index them
    BoundRect bounds;       // bounding box for region
    double baseX, baseY;    // the base location in high precision
  };
  struct Segment {
    uint32_t numPoints;
    uint32_t type : 8;
    uint32_t unused : 24;
    //    float xMin, xMax, yMin, yMax;  // bounding box
    // TODO: remove baseLocX and Y from segment, move to region
    // high precision base, all points can be relative
    double baseLocX, baseLocY;  // if (0,0) this is unused
  };

 private:
  BlockMapHeader* blockMapHeader;
  Segment* segments;
  float* points;
  static constexpr uint32_t version = 0x00010000;
  typedef void (BlockMapLoader::*Method)();
  const static Method methods[];

 public:
  void init(const uint64_t* mem, uint64_t size);
  void init(uint32_t numLists, uint32_t numPoints);
  // fast load a blockmap from a .bml file
  BlockMapLoader(const char filename[]);

  // load and convert an ESRI .shp to BlockMap format
  BlockMapLoader(const char filename[], const char[]);
  static BlockMapLoader loadCompressed(const char filename[]);

  // save a fast blockmap file
  void save(const char filename[]);

  void filterX(double xMin, double xMax);
  void filterY(double yMin, double yMax);
  void filter(double xMin, double xMax, double yMin, double yMax);

  // methods for different types of segments

  void methodPolygon();
  void methodPolyline();
  uint64_t sum() const;
  void deltaEncode();
  void deltaUnEncode();
  void dumpSegment(uint32_t seg);
};
