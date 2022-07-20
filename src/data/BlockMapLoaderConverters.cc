#include <iostream>
#include <vector>

#include "data/BlockMapLoader.hh"
#include "libshape/shapefil.h"
#include "maps/MapNames.hh"
#include "util/Ex.hh"
#include "maps/Geometry.hh"
using namespace std;



// extra parameter calls loader from ESRI Shapefile
// TODO: BlockMapLoader BlockMapLoader::loadESRI(const char filename[])
BlockMapLoader BlockMapLoader::loadFromESRI(const char filename[], vector<NamedMapEntry> sortedEntries, bool toggleDateLine) {
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

  uint32_t numRegionContainers = 0;
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

  BlockMapLoader bml(sizeof(BlockMapHeader) + numRegionContainers * sizeof(RegionContainer) + nEntities * sizeof(Region) +
                         numSegments * sizeof(Segment) + numPoints * 2 * sizeof(float),
                     version);
  // first bytes past standard header is the header specific to this file format
  bml.blockMapHeader = (BlockMapHeader*)bml.getSpecificHeader();
  // next, get the location of the segments
  bml.blockMapHeader->bounds.xMin = minBounds[0];
  bml.blockMapHeader->bounds.xMax = maxBounds[0];
  bml.blockMapHeader->bounds.yMin = minBounds[1];
  bml.blockMapHeader->bounds.yMax = maxBounds[1];
  bml.blockMapHeader->numRegionContainers = numRegionContainers;
  bml.blockMapHeader->numRegions = nEntities;
  bml.blockMapHeader->numSegments = numSegments;
  bml.blockMapHeader->numPoints = numPoints;
  bml.regionContainers = (RegionContainer*)((char*)bml.blockMapHeader + sizeof(BlockMapHeader));
  bml.regions = (Region*)((char*)bml.regionContainers + 
                            bml.blockMapHeader->numRegionContainers * sizeof(RegionContainer));
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

  for (uint32_t i = 0; i < sortedEntries.size(); i++) {
    uint32_t offset = sortedEntries[i].countyEntry.offset;
    int* start = shapes[offset]->panPartStart;
    if (start == nullptr && shapes[offset]->nParts != 0) {
      std::cerr << "error null list of offsets for parts panpartstart\n";
      // TODO: use exception
      throw(Ex1(Errcode::FILE_READ));
    }
    bml.regions[i].segmentStart = segCount;
    BoundRect& bounds = bml.regions[i].bounds;
    bounds.xMin = shapes[offset]->dfXMin; // copying bounds from ESRI into bml
    bounds.xMax = shapes[offset]->dfXMax;
    bounds.yMin = shapes[offset]->dfYMin;
    bounds.yMax = shapes[offset]->dfYMax;
    bml.regions[i].baseX = shapes[offset]->padfX[0]; // baseX baseY stored as double
    bml.regions[i].baseY = shapes[offset]->padfY[0]; // for future accuracy = ESRI map
    for (uint32_t j = 0; j < shapes[offset]->nParts; j++, segCount++) {
      bml.segments[segCount].type = shapes[offset]->nSHPType; // should be a closed polygon
      uint32_t numSegPoints;
      if (j == shapes[offset]->nParts - 1)
        numSegPoints = shapes[offset]->nVertices - start[j] - 1; // last point is a dup of the first
      else
        numSegPoints = start[j + 1] - start[j] - 1; // last point is a dup of the first
        // at this point numPoints = number of points in your polygon
      bml.segments[segCount].numPoints = numSegPoints;
      // uint32_t startOffset = pointOffset;
      bml.segments[segCount].start = pointOffset;
      Point center = centroid(shapes[offset]->padfX + start[j],shapes[offset]->padfY + start[j], numSegPoints, lonThresh);
      #if 0
      if(center.x  >-65)
      {
        cout << "Hello"<<endl;
      }
      #endif
      for (uint32_t k = 0; k < numSegPoints; k++) { // number of points in the segment
      //TODO: if we delta-compress this, it will be a)more accurate and b)compress WAY better
      //  float dx = shapes[offset]->padfX[start[j] + k] - baseX; // doing everything as deltas off baseX,baseY is more accurate
      //  float dy = shapes[offset]->padfY[start[j] + k] - baseY;
        float x = shapes[offset]->padfX[start[j] + k]; // x coord
        if (toggleDateLine)
          if (x > lonThresh)
            x -= 360.0f;
        float y = shapes[offset]->padfY[start[j] + k]; // y coord
        bml.points[pointOffset++] = x;
        bml.points[pointOffset++] = y;
      }
      //Point center = centroid(&bml.points[pointOffset - (numSegPoints*2)], numSegPoints);
      bml.points[pointOffset++] = center.x;
      bml.points[pointOffset++] = center.y;
      // TODO: currently broken
      //vector<Range> ranges = checkIntersections((Point*)(&bml.points[pointOffset - 2 - 2*numSegPoints]), numSegPoints, center);
      
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
      vector<Range> ranges = checkIntersections((Point*)(&bml.points[pointOffset - 2 - 2*numSegPoints]), numSegPoints, center);
      
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
  #endif

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

//===========================================================================
// buildCountyNames.cc moved here
//===========================================================================

const char* FIPsToPostal[100];
BLHashMap<uint32_t> postalToFIPs(200, 128, 128);

#if 0
const char countyName[] = "Alice\nBob\nCalvin\nDov\n";
NamedEntry counties[] = { 
  {0, 5, ENT_COUNTY, FEAT_LOCAL, 0}, // Alice
  {6, 3, ENT_COUNTY, FEAT_LOCAL, 1}, // Bob
  {10, 6, ENT_COUNTY, FEAT_LOCAL, 2}, // Calvin
  {17, 3, ENT_COUNTY, FEAT_LOCAL, 3} // Dov
};
#endif

void build2wayPostal(const char name[], uint32_t FIPs) {
  postalToFIPs.add(name, FIPs);
  FIPsToPostal[FIPs] = name;
}

void buildPostalAbbr() {
  build2wayPostal("AL", 1);
  build2wayPostal("AK", 2);
  build2wayPostal("AZ", 4);
  build2wayPostal("AR", 5);
  build2wayPostal("CA", 6);
  build2wayPostal("CO", 8);
  build2wayPostal("CT", 9);
  build2wayPostal("DE", 10);
  build2wayPostal("DC", 11);
  build2wayPostal("FL", 12);
  build2wayPostal("GA", 13);
  build2wayPostal("HI", 15);
  build2wayPostal("ID", 16);
  build2wayPostal("IL", 17);
  build2wayPostal("IN", 18);
  build2wayPostal("IA", 19);
  build2wayPostal("KS", 20);
  build2wayPostal("KY", 21);
  build2wayPostal("LA", 22);
  build2wayPostal("ME", 23);
  build2wayPostal("MD", 24);
  build2wayPostal("MA", 25);
  build2wayPostal("MI", 26);
  build2wayPostal("MN", 27);
  build2wayPostal("MS", 28);
  build2wayPostal("MO", 29);
  build2wayPostal("MT", 30);
  build2wayPostal("NE", 31);
  build2wayPostal("NV", 32);
  build2wayPostal("NH", 33);
  build2wayPostal("NJ", 34);
  build2wayPostal("NM", 35);
  build2wayPostal("NY", 36);
  build2wayPostal("NC", 37);
  build2wayPostal("ND", 38);
  build2wayPostal("OH", 39);
  build2wayPostal("OK", 40);
  build2wayPostal("OR", 41);
  build2wayPostal("PA", 42);
  build2wayPostal("PR", 72);
  build2wayPostal("RI", 44);
  build2wayPostal("SC", 45);
  build2wayPostal("SD", 46);
  build2wayPostal("TN", 47);
  build2wayPostal("TX", 48);
  build2wayPostal("UT", 49);
  build2wayPostal("VT", 50);
  build2wayPostal("VA", 51);
  build2wayPostal("VI", 78);
  build2wayPostal("WA", 53);
  build2wayPostal("WV", 54);
  build2wayPostal("WI", 55);
  build2wayPostal("WY", 56);
}

struct compareNamedEntries {
  bool operator() (const NamedMapEntry &a, const NamedMapEntry &b) { 
    return (strcmp(a.countyName, b.countyName) < 0);
  }
};

// building map dictionary from .dbf
// https://support.esri.com/en/technical-article/000010834
vector<NamedMapEntry> BlockMapLoader::buildMapDict(const char filename[]) {
  buildPostalAbbr();
  uint32_t itemCount = 10000;
  uint32_t symbolCapacity = 100000;
  BLHashMap<MapEntry> mapDict = BLHashMap<MapEntry>(symbolCapacity, itemCount, itemCount); 
  // loading in from .dbf file
  DBFHandle dbf = DBFOpen(filename, "rb");

  int recordCount = DBFGetRecordCount(dbf);
  int FID = DBFGetFieldIndex(dbf, "FID");  // get index of this field
  int NAME = DBFGetFieldIndex(dbf, "NAME"); // county name
  int STATE_NAME = DBFGetFieldIndex(dbf, "STATE_NAME"); // state name of county
  int STATE_FIPS = DBFGetFieldIndex(dbf, "STATE_FIPS"); // state id

   vector<NamedMapEntry> sortedEntries(recordCount);

  for (int i = 0; i < recordCount; i++) {
    const int fieldNum = DBFReadIntegerAttribute(dbf, i, FID);
    // array size determined from metadata: max length + 1 (for null)
    char county[24]; 
    strcpy(county, FIPsToPostal[DBFReadIntegerAttribute(dbf, i, STATE_FIPS)]);
    strcat(county, DBFReadStringAttribute(dbf, i, NAME));
    // now county = state abbr + county name. e.g. NYKings
    char state[21]; strcpy(state, DBFReadStringAttribute(dbf, i, STATE_NAME));
    const int stateNum = DBFReadIntegerAttribute(dbf, i, STATE_FIPS);
    cout << fieldNum << '\t'
         << county << '\t'
         << state << '\t'
         << stateNum << '\n';

    sortedEntries[i].countyEntry = MapEntry{ENT_COUNTY, FEAT_LOCAL, static_cast<uint32_t>(fieldNum-1)};
    sortedEntries[i].stateEntry = MapEntry{ENT_STATE, FEAT_STATE, static_cast<uint32_t>(fieldNum-1)};
    strcpy(sortedEntries[i].countyName, county);
    strcpy(sortedEntries[i].stateName, state);
  #if 0
//    uint32_t symbolSize = mapDict.getSymbolSize();
    mapDict.add(county, MapEntry{
      //symbolSize, (uint32_t)strlen(county), 
      ENT_COUNTY, FEAT_LOCAL, static_cast<uint32_t>(fieldNum-1)}); //warning: both static casts are narrowing int to uint32_t
    if (mapDict.get(state) == nullptr) {
//      symbolSize = mapDict.getSymbolSize();
      mapDict.add(state, MapEntry{
        ENT_STATE, FEAT_STATE, static_cast<uint32_t>(stateNum-1)}); // decrement by 1 since FIPS starts at 1, not 0
    }
  #endif
  }
  sort(sortedEntries.begin(), sortedEntries.end(), compareNamedEntries());
  for (int i = 0; i < recordCount; i++) {
    mapDict.add(sortedEntries[i].countyName, sortedEntries[i].countyEntry);
    #if 0
    if (mapDict.get(sortedEntries[i].stateName) == nullptr) {
      mapDict.add(sortedEntries[i].stateName, sortedEntries[i].stateEntry);
    }
    #endif
  }
  mapDict.writeFile("res/maps/uscounties.bdl");
  DBFClose(dbf);
  return sortedEntries;
}