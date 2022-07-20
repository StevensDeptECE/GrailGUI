//https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
//https://www.topcoder.com/thrive/articles/Geometry%20Concepts%20part%202:%20%20Line%20Intersection%20and%20its%20Applications
#include "maps/Geometry.hh"
#include <algorithm>
#include <numbers>
#include <cmath>
#include <vector>

//constexpr float inf = INFINITY;
constexpr uint32_t numBins = 1600;
constexpr double binSize = 2*numbers::pi/numBins;
constexpr double inverseBinSize = 1/binSize;

bool intersection(Line line1, Line line2) { // Returns the intersection point between two lines
  float x, y;
  float A1, A2, B1, B2, C1, C2;
  A1 = line1.end.y - line1.start.y;
  A2 = line2.end.y - line2.start.y;
  B1 = line1.start.x - line1.end.x;
  B2 = line2.start.x - line2.end.x;
  C1 = A1 * line1.start.x + B1 * line1.start.y;
  C2 = A1 * line2.start.x + B1 * line2.start.y;
  float det = A1 * B2 - A2 * B1;
  if(det !=0) {
    x = (B2 * C1 - B1 * C2)/det;
    y = (A1 * C2 - C1 * A2)/det;
  }
  else
  {
    return false;
  }
  if(x <= max(line1.end.x, line1.start.x) 
  && x >= min(line1.end.x, line1.start.x) 
  && y <= max(line1.end.y, line1.start.y) 
  && y >= min(line1.end.y, line1.start.y)) {
    return true;
  }
  else {
    return false;
  }
}
// x y x y x y ...
float area(float xy[], int numPoints) { // Calculates the area of a polygon
  Point* p = (Point*)xy;
  float area = 0;
  for(int i = 0; i < numPoints-1; i++)
  {
    area += (p[i].x * p[i+1].y - p[i+1].x * p[i].y);
  }
  area /= 2;
  return area;
}

Point centroid(const float xy[], const int numPoints){ // Calculates the centroid of a polygon
  Point* p = (Point*)xy;
  Point center(0,0);
  //float a = area(xy, numPoints);
  double a = 0;
  double cx = 0, cy = 0;
  double seg;
  int i;
  for (i = 0; i < numPoints-1; i++) {
    seg = (p[i].x * p[i+1].y - p[i+1].x * p[i].y);
    a += seg;
    cx += (p[i].x + p[i+1].x) * seg;
    cy += (p[i].y + p[i+1].y) * seg;
  }
  // SPECIAL CASE: Wrap-around back to i = 0;
  seg = (p[i].x * p[0].y - p[0].x * p[i].y);
  a += seg;
  cx += (p[i].x + p[0].x) * seg;
  cy += (p[i].y + p[0].y) * seg;

  a /= 2;
  cx /= 6*a;
  cy /= 6*a;
  return Point(cx, cy);
}
inline int getBin(Point centroid, Point p)
{
  double angle = atan2(p.y-centroid.y, p.x - centroid.x);
  if (angle < 0)
    angle += 2*numbers::pi;
  return (int)(angle * inverseBinSize);
}
inline void addToBin(const Point centroid, const Point a, const Point b, vector<const Line*> angleBins[], Line lineSegments[], int i){
    
    lineSegments[i] = Line(a, b);
    int startBin = getBin(centroid, lineSegments[i].start);
    int endBin = getBin(centroid, lineSegments[i].end);
    if(startBin > endBin)
    {
      swap(startBin, endBin);
    }
    for(int j = startBin; j<=endBin; j++)
    {
      angleBins[j].push_back(&lineSegments[i]);
    }
}
/**
 * @brief 
 * Checks whether there is an intersection between the line segement from the centroid to each point
 * and the other segments
 * Returns true if there are no intersections
 */
/*bool checkIntersections(const Point polygon[], int numPoints, Point centroid) {
  Line lineSegments[numPoints];
  bool intersections[numPoints];
  vector<const Line*> angleBins[numBins];
  for(int i = 0; i < numPoints-1; i++)
  {
    addToBin(centroid, polygon[i], polygon[i+1], angleBins, lineSegments, i);
  }
  addToBin(centroid, polygon[numPoints-1], polygon[0], angleBins, lineSegments, numPoints-1);
  for(int i = 0; i < numPoints; i++)
  {
    Line centLine(centroid, polygon[i]);
    int bin = getBin(centroid, polygon[i]);
    intersections[i] = true;
    for(const auto b: angleBins[bin])
    {
      if(intersection(centLine, *b))
      {
        intersections[i] = false;
        return false;
      }
    }
  }
  return true;
}*/
vector<Range> checkIntersections(const Point* polygon, int numPoints, Point centroid) {
  Line lineSegments[numPoints];
  bool intersections[numPoints];
  vector<Range> ranges;
  vector<const Line*> angleBins[numBins];
  for(int i = 0; i < numPoints-1; i++)
  {
    addToBin(centroid, polygon[i], polygon[i+1], angleBins, lineSegments, i);
  }
  addToBin(centroid, polygon[numPoints-1], polygon[0], angleBins, lineSegments, numPoints-1);
  for(int i = 0; i < numPoints; i++)
  {
    Line centLine(centroid, polygon[i]);
    int bin = getBin(centroid, polygon[i]);
    intersections[i] = false;
    for(const auto b: angleBins[bin])
    {
      if(intersection(centLine, *b))
      {
        intersections[i] = true;
      }
    }
  }
  bool inRange  = false;
  int start, end;
  for(int i = 0; i < numPoints; i++)
  {
    if(!inRange && intersections[i])
    {
      inRange = true;
      start = i;
    }
    else if(inRange && !intersections[i])
    {
      inRange = false;
      end = i-1;
      ranges.push_back(Range(start, end));
    }
  }
  return ranges;
}
//Point centroid<float>(float x[], float y[], int numPoints);
//Point centroid<double>(double x[], double y[], int numPoints);


Point centroid3(float xy[]) {
  return Point(xy[0] + xy[2] + xy[4], xy[1] + xy[3] + xy[5]);
}