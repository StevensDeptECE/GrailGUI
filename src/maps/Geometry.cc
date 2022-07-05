//https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
//https://www.topcoder.com/thrive/articles/Geometry%20Concepts%20part%202:%20%20Line%20Intersection%20and%20its%20Applications
#include "maps/Geometry.hh"

Point intersection(Line line1, Line line2) { // Returns the intersection point between two lines
  float x, y;
  float A1, A2, B1, B2, C1, C2;
  A1 = line1.end.y - line1.start.y;
  A2 = line2.end.y - line2.start.y;
  B1 = line1.start.x - line1.end.x;
  B2 = line2.start.x - line2.end.x;
  C1 = A1 * line1.start.x + B1 * line1.start.y;
  C2 = A1 * line2.start.x + B1 * line2.start.y;
  float det = A1 * B2 - A2 * B1;
  if(det != 0)  {
    x = (B2 * C1 - B1 * C2)/det;
    y = (A1 * C2 - C1 * A2)/det;
  }
  Point p;
  p.x = x;
  p.y = y;
  return p;
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


//Point centroid<float>(float x[], float y[], int numPoints);
//Point centroid<double>(double x[], double y[], int numPoints);


Point centroid3(float xy[]) {
  return Point(xy[0] + xy[2] + xy[4], xy[1] + xy[3] + xy[5]);
}