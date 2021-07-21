#include "opengl/Shapefile.hh"

#include <algorithm>

Shapefile::Shapefile(const char filename[]) {
  // Open shapefile using shapelib
  shapeHandle = SHPOpen(filename, "rb");
  if (shapeHandle == nullptr) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  // Load basic information
  nEntities = shapeHandle->nRecords;
  nShapeType = shapeHandle->nShapeType;
  for (int i = 0; i < 4; i++) {
    minBounds[i] = shapeHandle->adBoundsMin[i];
    maxBounds[i] = shapeHandle->adBoundsMax[i];
  }
}

Shapefile::~Shapefile() {
  // If shape vector has been initialized, free shape pointers
  for (int i = 0; i < shapeObjects.size(); i++) {
    SHPDestroyObject(shapeObjects[i]);
  }

  // Close handle to shapefile
  SHPClose(shapeHandle);
}

void Shapefile::init() {
  // Load all shapes at once
  shapeObjects.reserve(nEntities);
  SHPObject* shapeComp;
  for (int i = 0; i < nEntities; i++) {
    shapeComp = SHPReadObject(shapeHandle, i);
    if (!shapeComp) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      break;
    }

    // Load each shape into vector
    shapeObjects.push_back(shapeComp);
  }
}

double* Shapefile::getMinBounds() { return minBounds; }

double* Shapefile::getMaxBounds() { return maxBounds; }

SHPObject* Shapefile::getShape(int index) { return shapeObjects[index]; }

std::vector<SHPObject*> Shapefile::getShapeVector() { return shapeObjects; }
