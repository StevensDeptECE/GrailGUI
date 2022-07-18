//https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>
using namespace std;

struct Point {
  float x,y;
  Point(float x, float y) : x(x), y(y) {}
  Point() {}
};
struct Line {
  Point start, end;
  Line(Point start, Point end) : start(start), end(end){};
  Line() {}
};
struct Range {
  int start, end;
  Range(int start, int end) : start(start), end(end) {
    if(start > end)
    {
      swap(start, end);
    }
  }
  Range() {}
};

vector<Range> checkIntersections(const Point* polygon, int numPoints, Point centroid);
Point centroid(const float xy[], const int n);

template<typename Precision>
Point centroid(Precision x[], Precision y[], int numPoints, Precision lonThresh){
  double signedArea = 0;
  double cx = 0, cy = 0;
  double seg;
  double cxseg;
  double cyseg; 
  //std::ofstream f("dammit.txt");
  //f << "i" << "\t" << "x[i]" << "\t" << "y[i]" << "\t" << "seg" << "\t" << "cxseg" << "\t" << "cyseg" << "\n";
  for(int i = 0; i < numPoints; i++)
  {
    if(x[i] > lonThresh)
      x[i] -= 360;
  }
  int i;
  for (i = 0; i < numPoints-1; i++) {
    seg = (x[i] * y[i+1] - x[i+1] * y[i]);
    signedArea += seg;
    cxseg = (x[i] + x[i+1]) * seg;
    cyseg = (y[i] + y[i+1]) * seg;
    cx += cxseg;
    cy += cyseg;
    //f << i << setw(13) << x[i] << setw(13) << y[i] << setw(13) << seg << setw(13) << cxseg << setw(13) << cyseg << '\n';
  }
  seg = (x[i] * y[0] - x[0] * y[i]);
  signedArea += seg;
  cxseg = (x[i] + x[0]) * seg;
  cyseg = (y[i] + y[0]) * seg;
  cx += cxseg;
  cy += cyseg;
  //f << i << setw(13) << x[i] << setw(13) << y[i] << setw(13) << seg << setw(13) << cxseg << setw(13) << cyseg << '\n';
  signedArea /= 2;
  //f << "signedArea: " << seg << "\tcx=" << cx << "\tcy=" << cy << std::endl;
  cx /= 6*signedArea;
  cy /= 6*signedArea;
  //f << "centroid: cx=" << cx << "\t cy=" << cy << std::endl;
  return Point(cx, cy);
}



Point centroid3(float xy[]);
Point centroid3(float x[], float y[]);
