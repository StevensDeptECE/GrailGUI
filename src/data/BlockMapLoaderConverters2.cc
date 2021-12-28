#include <iostream>
#include <vector>

#include "data/BlockMapLoader2.hh"
#include "libshape/shapefil.h"
#include "util/Ex.hh"
using namespace std;

// extra parameter calls loader from ESRI Shapefile
// TODO: BlockMapLoader BlockMapLoader::loadESRI(const char filename[])
BlockMapLoader BlockMapLoader::loadFromESRI(const char filename[]) {
  SHPHandle shapeHandle = SHPOpen(filename, "rb");
  // Load basic information
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;
  //cerr << "nEntities: " << nEntities << endl;

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

  BlockMapLoader bml(sizeof(BlockMapHeader) + nEntities * sizeof(Region) +
                     numSegments * sizeof(Segment) + numPoints * 8, version);
  // first bytes past standard header is the header specific to this file format
  bml.blockMapHeader = (BlockMapHeader*)bml.getSpecificHeader();
  // next, get the location of the segments
  bml.blockMapHeader->bounds.xMin = minBounds[0];
  bml.blockMapHeader->bounds.xMax = maxBounds[0];
  bml.blockMapHeader->bounds.yMin = minBounds[1];
  bml.blockMapHeader->bounds.yMax = maxBounds[1];
  bml.blockMapHeader->numRegionContainers = 0;
  bml.blockMapHeader->numRegions = nEntities;
  bml.blockMapHeader->numSegments = numSegments;
  bml.blockMapHeader->numPoints = numPoints;
  bml.regions = (Region*)((char*)bml.blockMapHeader + sizeof(BlockMapHeader));
  bml.segments =
      (Segment*)((char*)bml.regions + bml.blockMapHeader->numRegions * sizeof(Region));

  // last, points are a block of floating point numbers
  bml.points =
      (float*)((char*)bml.segments + bml.blockMapHeader->numSegments * sizeof(Segment));
  uint32_t numDups = 0;
  uint32_t segCount = 0;
  //  float* xPoints = points;  // all x points are together in a single block
  //  float* yPoints = points + numPoints;  // same for y
  uint32_t pointOffset = 0;  // track current point
  uint32_t exactMatch = 0;
  for (uint32_t i = 0; i < shapes.size(); i++) {
    int* start = shapes[i]->panPartStart;
    if (start == nullptr && shapes[i]->nParts != 0) {
      std::cerr << "error null list of offsets for parts panpartstart\n";
      // TODO: use exception
      throw(Ex1(Errcode::FILE_READ));
    }
    // regions[i].numPoints =
    BoundRect& bounds = bml.regions[i].bounds;
    bounds.xMin = shapes[i]->dfXMin;
    bounds.xMax = shapes[i]->dfXMax;
    bounds.yMin = shapes[i]->dfYMin;
    bounds.yMax = shapes[i]->dfYMax;
    bml.regions[i].baseX = shapes[i]->padfX[0];
    bml.regions[i].baseY = shapes[i]->padfY[0];
    for (uint32_t j = 0; j < shapes[i]->nParts; j++, segCount++) {
      bml.segments[segCount].type = shapes[i]->nSHPType;
      uint32_t numPoints;
      if (j == shapes[i]->nParts - 1)
        numPoints = shapes[i]->nVertices - start[j];
      else
        numPoints = start[j + 1] - start[j];
      bml.segments[segCount].numPoints = numPoints;
      uint32_t startOffset = pointOffset;
      for (uint32_t k = 0; k < numPoints; k++) {
        bml.points[pointOffset++] = shapes[i]->padfX[start[j] + k];
        bml.points[pointOffset++] = shapes[i]->padfY[start[j] + k];
      }
      if (approxeqpt(bml.points[startOffset], bml.points[startOffset + 1],
                     bml.points[pointOffset - 2], bml.points[pointOffset - 1])) {
        if (bml.points[startOffset] == bml.points[pointOffset - 2] &&
            bml.points[startOffset + 1] == bml.points[pointOffset - 1])
          exactMatch++;
        pointOffset -= 2;  // remove last point, it's the same
        // cerr << "Removing final point of polygon, same as first point\n";
        numDups++;
        bml.segments[segCount].numPoints--;
      }
    }
  }

  cerr << "Removed " << numDups << " final points of polygons\n";
  bml.blockMapHeader->numPoints -= numDups;
  bml.size -= numDups * 2 * sizeof(float);
  for (auto shape : shapes) SHPDestroyObject(shape);
  SHPClose(shapeHandle);

  cerr << "Exact matches found:" << exactMatch << '\n';
  return bml;
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
