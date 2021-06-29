#include <iostream>
#include <vector>

#include "data/BlockMapLoader.hh"
#include "libshape/shapefil.h"
using namespace std;

// extra parameter calls loader from ESRI Shapefile
// TODO: BlockMapLoader BlockMapLoader::loadESRI(const char filename[])
BlockMapLoader::BlockMapLoader(
    const char filename[],
    const char[]) {  // TODO:}: BlockLoader( loadESRI(filename) ){
  SHPHandle shapeHandle = SHPOpen(filename, "rb");
  // Load basic information
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;
  cerr << "nEntities: " << nEntities << endl;

  double minBounds[4], maxBounds[4];

  for (int i = 0; i < 4; i++) {
    minBounds[i] = shapeHandle->adBoundsMin[i];
    maxBounds[i] = shapeHandle->adBoundsMax[i];
  }

  uint64_t numPoints = 0;
  uint32_t numSegments = 0;
  vector<SHPObject*> shapes(nEntities);
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    numPoints += shape->nVertices;
    numSegments += shape->nParts;
    shapes[i] = shape;
  }
  BlockLoader::init(sizeof(BlockMapHeader) + nEntities * sizeof(Region) +
                        numSegments * sizeof(Segment) + numPoints * 8,
                    Type::gismap, version);
  // first bytes past standard header is the header specific to this file format
  blockMapHeader = (BlockMapHeader*)((char*)mem + getHeaderSize());
  // next, get the location of the segments
  blockMapHeader->bounds.xMin = minBounds[0];
  blockMapHeader->bounds.xMax = maxBounds[0];
  blockMapHeader->bounds.yMin = minBounds[1];
  blockMapHeader->bounds.yMax = maxBounds[1];
  blockMapHeader->numRegionContainers = 0;
  blockMapHeader->numRegions = nEntities;
  blockMapHeader->numSegments = numSegments;
  blockMapHeader->numPoints = numPoints;
  regions = (Region*)((char*)blockMapHeader + sizeof(BlockMapHeader));
  segments =
      (Segment*)((char*)regions + blockMapHeader->numRegions * sizeof(Region));

  // last, points are a block of floating point numbers
  points =
      (float*)((char*)segments + blockMapHeader->numSegments * sizeof(Segment));
  uint32_t segCount = 0;
  //  float* xPoints = points;  // all x points are together in a single block
  //  float* yPoints = points + numPoints;  // same for y
  uint32_t pointOffset = 0;  // track current point
  for (uint32_t i = 0; i < shapes.size(); i++) {
    int* start = shapes[i]->panPartStart;
    if (start == nullptr && shapes[i]->nParts != 0) {
      cerr
          << "error null list of offsets for parts panpartstart\n";  // TODO:
                                                                     // use
                                                                     // exception
      return;
    }
    // regions[i].numPoints =
    regions[i].bounds.xMin = shapes[i]->dfXMin;
    regions[i].bounds.xMax = shapes[i]->dfXMax;
    regions[i].bounds.yMin = shapes[i]->dfYMin;
    regions[i].bounds.yMax = shapes[i]->dfYMax;
    regions[i].baseX = shapes[i]->padfX[0];
    regions[i].baseY = shapes[i]->padfY[0];
    for (uint32_t j = 0; j < shapes[i]->nParts; j++, segCount++) {
      segments[segCount].type = shapes[i]->nSHPType;
      uint32_t numPoints;
      if (j == shapes[i]->nParts - 1)
        numPoints = shapes[i]->nVertices - start[j];
      else
        numPoints = start[j + 1] - start[j];
      segments[segCount].numPoints = numPoints;
      for (uint32_t k = 0; k < numPoints; k++) {
        points[pointOffset++] = shapes[i]->padfX[start[j] + k];
        points[pointOffset++] = shapes[i]->padfY[start[j] + k];
      }
    }
  }
  for (auto shape : shapes) SHPDestroyObject(shape);
  SHPClose(shapeHandle);
}

void BlockMapLoader::filterX(double xMin, double xMax) {
#if 0
  int j = 0;
  for (uint32_t i = 0; i < specificHeader->numLists; i++) {
    uint32_t listLen = listSizes[i];
    uint32_t selectedThisList = 0;
    for (int i = 0; i < listLen; i += 2) {
      if (points[i] >= xMin && points[i] <= xMax) {
        shape[j++] = shape[i];      // copy in x
        shape[j++] = shape[i + 1];  // and y (point is within bounds
        selectedThisList++;
      }
    }
    listSizes[i] = selectedThisList;
  }
#endif
}

void BlockMapLoader::filterY(double yMin, double yMax) {}

void BlockMapLoader::filter(double xMin, double xMax, double yMin,
                            double yMax) {}
