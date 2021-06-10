#include "opengl/BlockMapLoader.hh"
#include "libshape/shapefil.h"
#include <iostream>
#include <array>
using namespace std;

// extra parameter calls loader from ESRI Shapefile
void BlockMapLoader::BlockMapLoader(const char filename[], const char[]) {
	SHPHandle shapeHandle = SHPOpen(filename, "rb");
	// Load basic information
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;
	double minBounds[4], maxBounds[4];

  for (int i = 0; i < 4; i++) {
    minBounds[i] = shapeHandle->adBoundsMin[i];
    maxBounds[i] = shapeHandle->adBoundsMax[i];
  }

	uint32_t numPoints = 0;
	array<SHPObject*> shapes(nEntities);
  for (int i = 0; i < nEntities; i++) {
		SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shapeComp == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
		numPoints += shape->nVertices;
		shapes.push_back(shape);
	}

	init(sizeof(SpecificHeader) + numLists * sizeof(Segment) + numPoints * 8, Type::gismap, version);
	segments = (Segment*)((char*)mem + getHeaderSize());  // list of segments is next
	points = (float*)((char*)segments + numPoints * sizeof(Segment));
	uint32_t sizeSoFar = 0;
	for (uint32_t i = 0; i < shapes->size(); i++) {
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
			float* yPoints = points + numPoints; // y starts after all x
			sizeSoFar += numPoints * 2;

			for (uint32_t j = 0; j < numPoints; j++) {
				xPoints[j] = shapePtr->padfX[j];
				yPoints[j] = shapePtr->padfY[j];
			}
		}
		for (auto shape : shapes)
			SHPClose(shape);
	}
}

void BlockMapLoader::filterX(double xMin, double xMax) {
	int j = 0;
	for (uint32_t i = 0; i < numLists; i++) {
		uint32_t listLen = listSizes[i];
		uint32_t selectedThisList = 0;
		for (int i = 0; i < listLen; i += 2) {
			if (points[i] >= xMin && points[i] <= xMax) {
				shape[j++] = shape[i];  // copy in x
				shape[j++] = shape[i+1];// and y (point is within bounds
				selectedThisList++;
			}
		}
		listSizes[i] = selectedThisList;
	}
}

void BlockMapLoader::filterY(double yMin, double yMax) {
}

void BlockMapLoader::filter(double xMin, double xMax,
														double yMin, double yMax) {
	
}
