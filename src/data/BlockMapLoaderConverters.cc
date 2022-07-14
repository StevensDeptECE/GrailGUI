#include <iostream>
#include <vector>

#include "data/BlockMapLoader.hh"
#include "libshape/shapefil.h"
#include "util/Ex.hh"
#include "maps/Geometry.hh"
using namespace std;

// extra parameter calls loader from ESRI Shapefile
// TODO: BlockMapLoader BlockMapLoader::loadESRI(const char filename[])
BlockMapLoader BlockMapLoader::loadFromESRI(const char filename[], bool toggleDateLine) {
  SHPHandle shapeHandle = SHPOpen(filename, "rb");
  // Load basic information
  int nEntities = shapeHandle->nRecords;
  // int nShapeType = shapeHandle->nShapeType;
  // cerr << "nEntities: " << nEntities << endl;

  double minBounds[4], maxBounds[4];
  const double lonThresh = 100;

  for (int i = 0; i < 4; i++) {
    minBounds[i] = shapeHandle->adBoundsMin[i];
    maxBounds[i] = shapeHandle->adBoundsMax[i];
  }

  uint64_t numPoints = 0;
  uint32_t numSegments = 0;
  vector<SHPObject*> shapes(nEntities); // each county shape is a list of polygons
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    numPoints += shape->nVertices; // ESRI has 1 extra point per polygon wasted (duplicate of start)
    numSegments += shape->nParts;
    shapes[i] = shape;
  }
  // but we want 1 extra point for each polygon for centroid: net NO CHANGE
  //NO EXTRA POINTS???? numPoints += numSegments; // adding 1 extra point for centroid of polygon

  BlockMapLoader bml(sizeof(BlockMapHeader) + nEntities * sizeof(Region) +
                         numSegments * sizeof(Segment) + numPoints * 2 * sizeof(float),
                     version);//TODO: Find why 32 bytes are missing
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
  bml.segments = (Segment*)((char*)bml.regions +
                            bml.blockMapHeader->numRegions * sizeof(Region));

  // last, points are a block of floating point numbers
  bml.points = (float*)((char*)bml.segments +
                        bml.blockMapHeader->numSegments * sizeof(Segment));
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
    bml.regions[i].segmentStart = segCount;
    BoundRect& bounds = bml.regions[i].bounds;
    bounds.xMin = shapes[i]->dfXMin; // copying bounds from ESRI into bml
    bounds.xMax = shapes[i]->dfXMax;
    bounds.yMin = shapes[i]->dfYMin;
    bounds.yMax = shapes[i]->dfYMax;
    bml.regions[i].baseX = shapes[i]->padfX[0]; // baseX baseY stored as double
    bml.regions[i].baseY = shapes[i]->padfY[0]; // for future accuracy = ESRI map
    for (uint32_t j = 0; j < shapes[i]->nParts; j++, segCount++) {
      bml.segments[segCount].type = shapes[i]->nSHPType; // should be a closed polygon
      uint32_t numSegPoints;
      if (j == shapes[i]->nParts - 1)
        numSegPoints = shapes[i]->nVertices - start[j] - 1; // last point is a dup of the first
      else
        numSegPoints = start[j + 1] - start[j] - 1; // last point is a dup of the first
        // at this point numPoints = number of points in your polygon
      bml.segments[segCount].numPoints = numSegPoints;
      // uint32_t startOffset = pointOffset;
      bml.segments[segCount].start = pointOffset;
      Point center = centroid(shapes[i]->padfX + start[j],shapes[i]->padfY + start[j], numSegPoints, lonThresh);
      #if 0
      if(center.x  >-65)
      {
        cout << "Hello"<<endl;
      }
      #endif
      for (uint32_t k = 0; k < numSegPoints; k++) { // number of points in the segment
      //TODO: if we delta-compress this, it will be a)more accurate and b)compress WAY better
      //  float dx = shapes[i]->padfX[start[j] + k] - baseX; // doing everything as deltas off baseX,baseY is more accurate
      //  float dy = shapes[i]->padfY[start[j] + k] - baseY;
        float x = shapes[i]->padfX[start[j] + k]; // x coord
        if (toggleDateLine)
          if (x > lonThresh)
            x -= 360.0f;
        float y = shapes[i]->padfY[start[j] + k]; // y coord
        bml.points[pointOffset++] = x;
        bml.points[pointOffset++] = y;
      }
      //Point center = centroid(&bml.points[pointOffset - (numSegPoints*2)], numSegPoints);
      bml.points[pointOffset++] = center.x;
      bml.points[pointOffset++] = center.y;
      //bml.points[pointOffset++] = (bounds.xMin + bounds.xMin)/2;
      //bml.points[pointOffset++] = (bounds.yMin + bounds.yMin)/2;
      #if 0
      if (approxeqpt(bml.points[startOffset], bml.points[startOffset + 1],
                     bml.points[pointOffset - 2],
                     bml.points[pointOffset - 1])) {
        if (bml.points[startOffset] == bml.points[pointOffset - 2] &&
            bml.points[startOffset + 1] == bml.points[pointOffset - 1])
          exactMatch++;
        pointOffset -= 2;  // remove last point, it's the same
        // cerr << "Removing final point of polygon, same as first point\n";
        numDups++;
        bml.segments[segCount].numPoints--;
      }
      #endif
    }
  }

  #if 0
  // cerr << "Removed " << numDups << " final points of polygons\n";
  uint32_t numFloatsRemoved = numDups * 2;
  bml.blockMapHeader->numPoints -= numFloatsRemoved;
  bml.size -= numFloatsRemoved * sizeof(float);
  #endif
  for (const auto& shape : shapes) SHPDestroyObject(shape);
  SHPClose(shapeHandle);

  // cout << "num points=" << bml.getNumPoints() << '\n';
  //   constexpr float small = 5;
  //   uint64_t countextremepoints = 0;
  //   for (uint32_t i = 0, j = 0; i < bml.getNumPoints(); i++, j += 2) {
  //     if (abs(bml.points[j]) < small || abs(bml.points[j+1]) < small) {
  //       countextremepoints++;
  // //      cout << "found zero " << i << "\n";
  //     }
  //   }
  // //cout << "Found " << countextremepoints << " points outside range\n";
  // //cerr << "Exact matches found:" << exactMatch << '\n';
  // uint32_t numPointsInSegments = 0;
  // for (int i = 0; i < bml.getNumSegments(); i++)
  //   numPointsInSegments += bml.segments[i].numPoints;
  // cout << "number of points in segments=" << numPointsInSegments << '\n';
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
