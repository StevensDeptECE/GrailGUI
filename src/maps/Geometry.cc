//https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
#include "maps/Geometry.hh"


// x y x y x y ...
float area(float xy[], int numPoints) {
  Point* p = (Point*)xy;
  float area = 0;
  for(int i = 0; i < numPoints-1; i++)
  {
    area += (p[i].x * p[i+1].y - p[i+1].x * p[i].y);
  }
  area /= 2;
  return area;
}

Point centroid(float xy[], int numPoints){
  Point* p = (Point*)xy;
  Point center(0,0);
  //float a = area(xy, numPoints);
  float a = 0;
  float cx = 0, cy = 0;
  for (int i = 0; i < numPoints-1; i++) {
    float seg = (p[i].x * p[i+1].y - p[i+1].x * p[i].y);
    a += seg;
    cx += (p[i].x + p[i+1].x) * seg;
    cy += (p[i].y + p[i+1].y) * seg;
  }

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