#include "Shapefile.hh"

#include <algorithm>

//TODO: Implement destructor for shape pointers
Shapefile::Shapefile(const char filename[]) {
  shapeHandle = SHPOpen(filename, "rb");
  if (shapeHandle == nullptr) {
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }

  nEntities = shapeHandle->nRecords;
  nShapeType = shapeHandle->nShapeType;
  for (int i = 0; i < 4; i++) {
    minBounds[i] = shapeHandle->adBoundsMin[i];
    maxBounds[i] = shapeHandle->adBoundsMax[i];
  }
}

void Shapefile::init() {
  shapeObjects.reserve(nEntities);
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shapeComp = SHPReadObject(shapeHandle, i);
    if (!shapeComp) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      break;
    }

    shapeObjects.push_back(shapeComp);
  }
}

SHPObject* Shapefile::getShape(int index) {
  return shapeObjects[index];
}

std::vector<SHPObject*> Shapefile::getShapeVector() {
  return shapeObjects;
}
