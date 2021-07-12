#include "libshape/shapefil.h"
#include <iostream>
#include <vector>

using namespace std;

void loadESRIShapefile(const char filename[]) {
    SHPHandle shapeHandle = SHPOpen(filename, "rb");
    int nEntities = shapeHandle->nRecords;
    int nShapeType = shapeHandle->nShapeType;

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

    vector<uint32_t> startPoly(numSegments); // preallocate correct number of polygons
    vector<float> points(numPoints);         // preallocate correct number of points
    uint32_t pointOffset = 0;  // track current point
    for (uint32_t i = 0; i < shapes.size(); i++) {
        int* start = shapes[i]->panPartStart;
        if (start == nullptr && shapes[i]->nParts != 0) {
            cerr << "Error null list of offsets shape=" << i << " for parts panpartstart\n";
            // TODO: // exception
            return;
        }

        for (uint32_t j = 0; j < shapes[i]->nParts; j++) {
            uint32_t numPoints = j == shapes[i]->nParts - 1 ?
                shapes[i]->nVertices - start[j] : start[j + 1] - start[j];
            for (uint32_t k = 0; k < numPoints; k++) {
                points[pointOffset++] = shapes[i]->padfX[start[j] + k];
                points[pointOffset++] = shapes[i]->padfY[start[j] + k];
            }
        }
    }
    for (auto shape : shapes)
        SHPDestroyObject(shape);
    SHPClose(shapeHandle);
}

void loadESRIDBF(const char filename[]) {

}

int main() {
    string dir = getenv("GRAIL"); dir += "res/maps/";
    cout << "dir: " << dir << '\n';
    loadESRIShapefile(dir + "US_Counties.shp");
    loadDBF(dir + "US_Counties.dbf");
}
