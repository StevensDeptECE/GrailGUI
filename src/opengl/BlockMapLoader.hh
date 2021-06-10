#pragma once
#include "BlockLoader.hh"

class BlockMapLoader : public BlockLoader {
 public:
  struct SpecificHeader {
    uint32_t numLists;
    uint32_t unused;
  };

  struct Segment {
    uint32_t numPoints;
    uint32_t type : 8;
    uint32_t unused : 24;
    float xMin, xMax, yMin, yMax;  // bounding box

    // high precision base, all points can be relative
    double baseLocX, baseLocY;  // if (0,0) this is unused
  };

 private:
  SpecificHeader* specificHeader;
  Segment* segments;
  float* points;
  static constexpr uint32_t version = 0x00010000;
  typedef void (BlockMapLoader::*Method)();
  const static Method methods[];

 public:
  void init(uint32_t numLists, uint32_t numPoints);
  // fast load a blockmap from a .bml file
  BlockMapLoader(const char filename[]);

  // load and convert an ESRI .shp to BlockMap format
  BlockMapLoader(const char filename[], const char[]);

  // save a fast blockmap file
  void save(const char filename[]);

  void filterX(double xMin, double xMax);
  void filterY(double yMin, double yMax);
  void filter(double xMin, double xMax, double yMin, double yMax);

  // methods for different types of segments

  void methodPolygon();
  void methodPolyline();
};
