#include <iostream>
#include <vector>

#include "libshape/shapefil.h"
#include "opengl/BlockMapLoader.hh"
using namespace std;

// extra parameter calls loader from ESRI Shapefile
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
  vector<SHPObject*> shapes(nEntities);
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    numPoints += shape->nVertices;
    shapes[i] = shape;
  }

  BlockLoader::init(
      sizeof(SpecificHeader) + nEntities * sizeof(Segment) + numPoints * 8,
      Type::gismap, version);
  // first bytes past standard header is the header specific to this file format
  specificHeader = (SpecificHeader*)((char*)mem + getHeaderSize());
  // next, get the location of the segments
  specificHeader->numLists = nEntities;
  segments = (Segment*)((char*)specificHeader + sizeof(SpecificHeader));

  // last, points are a block of floating point numbers
  points =
      (float*)((char*)segments + specificHeader->numLists * sizeof(Segment));
  uint32_t sizeSoFar = 0;
  for (uint32_t i = 0; i < shapes.size(); i++) {
    segments[i].type = shapes[i]->nSHPType;
    segments[i].xMin = shapes[i]->dfXMin;
    segments[i].xMax = shapes[i]->dfXMax;
    segments[i].yMin = shapes[i]->dfYMin;
    segments[i].yMax = shapes[i]->dfYMax;

    uint32_t numPoints = segments[i].numPoints = shapes[i]->nVertices;
    if (numPoints > 0) {
      segments[i].baseLocX = shapes[i]->padfX[0];
      segments[i].baseLocY = shapes[i]->padfY[0];
      float* xPoints = points + sizeSoFar;
      float* yPoints = points + sizeSoFar + numPoints;  // y starts after all x
      sizeSoFar += numPoints * 2;

      for (uint32_t j = 0; j < numPoints; j++) {
        xPoints[j] = shapes[i]->padfX[j];
        yPoints[j] = shapes[i]->padfY[j];
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
