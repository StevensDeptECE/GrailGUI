#include "opengl/BlockMapLoader.hh"
#include "libshape/shapefil.h"
#include <iostream>
using namespace std;

void BlockMapLoader::loadShapefile(const char filename[]) {
	SHPHandle shapeHandle = SHPOpen(filename, "rb");
	// Load basic information
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;
	double minBounds[4], maxBounds[4];

  for (int i = 0; i < 4; i++) {
    minBounds[i] = shapeHandle->adBoundsMin[i];
    maxBounds[i] = shapeHandle->adBoundsMax[i];
  }

  vector<SHPObject*> shapeObjects(nEntities);

  for (int i = 0; i < nEntities; i++) {
		SHPObject* shapeComp = SHPReadObject(shapeHandle, i);
    if (shapeComp == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }

    // Load each shape into vector
    shapeObjects.push_back(shapeComp);
  }
	SHPClose(shapeHandle);
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
