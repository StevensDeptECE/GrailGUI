#pragma once
#include "data/BlockLoader2.hh"
#include "data/BoundRect.hh"
class BlockMapLoader : public BlockLoader {
 public:
  struct BlockMapHeader {
    uint32_t numNamedEntities;
    uint32_t numRegionContainers;
    uint32_t numRegions;
    uint32_t numSegments;
    uint32_t numPoints;
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
    uint32_t nameOffset;  // starting byte of the name;
    uint8_t len;          // length of the name (< 256 bytes)
    uint8_t entityType;   // entity cast as 8 bit number
    uint8_t type;         // feature type of name (could be used to choose font)
    uint32_t offset;      // if entityType=REGION_CONTAINER, then offset into
                          // regionContainers, ...
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
    uint32_t startPoints;   // starting index for points so you don't have to
                            // start from the beginning to index them
    BoundRect bounds;       // bounding box for region
    double baseX, baseY;    // the base location in high precision
  };
  struct Segment {
    uint32_t numPoints : 24;  // up to 16 million points
    uint32_t type : 8;        // TODO: Define what type of segments exist. polygons (islands, border) polyline (rivers, roads) etc
  };
  void mean(float* meanx, float* meany) const;
  
 private:
  BlockMapHeader* blockMapHeader;
  RegionContainer* regionContainers;
  Region* regions;
  Segment* segments;
  float* points;
  static constexpr uint16_t version = 0x0401;  // 0.4.0.1
  typedef void (BlockMapLoader::*Method)();
  const static Method methods[];

  constexpr static float eps = 1e-6;
  static bool approxeq(double a, double b) {
    return std::abs(b - a) < eps;  // TODO: is this good?
  }
  static bool approxeqpt(float x1, float y1, float x2, float y2) {
    return std::abs(x2 - x1) < eps && std::abs(y2 - y1) < eps;
  }
 public:
  //void init(const uint64_t* mem, uint64_t size);
  //void init(uint32_t numLists, uint32_t numPoints);
  // fast load a blockmap from a .bml file
  BlockMapLoader(const char filename[]);
  BlockMapLoader(uint64_t size, uint16_t version) : BlockLoader(size, Type::gismap, version) {}
  #if 0
  // is move constructor defined by default?
  BlockMapLoader(BlockMapLoader&& orig) :
   BlockLoader(orig),
   blockMapHeader(orig.blockMapHeader),
   regionContainers(orig.regionContainers),
   regions(orig.regions),
   segments(segments),
   points(points) {}
   #endif

  // load and convert an ESRI .shp to BlockMap format
  BlockMapLoader(const char filename[], const char[]);
  static BlockMapLoader loadFromESRI(const char filename[]);
  static BlockMapLoader loadCompressed(const char filename[]);
  // TODO: const RegionContainers* getRegionContainers() const { return
  // regionContainers; }

  const Region* getRegions() const { return regions; }
  const Segment* getSegments() const { return segments; }
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
  uint32_t getNumRegionContainers() const {
    return blockMapHeader->numRegionContainers;
  }
  const BlockMapHeader* getBlockMapHeader() const { return blockMapHeader; }
  uint32_t getNumRegions() const { return blockMapHeader->numRegions; }
  uint32_t getNumSegments() const { return blockMapHeader->numSegments; }
  uint32_t getNumPoints() const { return blockMapHeader->numPoints; }
  const float* getXPoints() const { return points; }
  const float* getYPoints() const { return points + blockMapHeader->numPoints; }
  const Segment& getSegment(uint32_t i) { return segments[i]; }
  static void diff(const BlockMapLoader& a, const BlockMapLoader& b);
};
