#include <iostream>
#include <cmath>
#include <algorithm>

struct MinMax {
  float min, max;
  MinMax(float v1, float v2) {
    if (v1 < v2)
      min = v1, max = v2;
    else
      min = v2, max = v1;
  }
  static bool intersects(MinMax a, MinMax b) {
    if (a.min > b.max || a.max < b.min)
      return false;
    return true;
  }
  static bool intersects(float a1, float a2, float b1, float b2) {
    return intersects(MinMax(a1,a2), MinMax(b1,b2));
  }
  // return true if b is between a1 and a2
  static bool intersects(float a1, float a2, float b) {
    MinMax mm(a1,a2);
    return b >= a1 && b <= a2;
  }

  // return true if b is between a1 and a2 (alternate version, compare for speed)
  static bool intersects(float a1, float a2, float b) {
    return (a1 < a2) ? b >= a1 && b <= a2 : b >= a2 && b <= a1;
  }
};
struct Point {
  float x,y;
  Point(float x, float y) : x(x), y(y) {}
  Point() {}
};
struct Seg {
  Point p1, p2;
  Seg(Point p1, Point p2) : p1(p1), p2(p2) {}

  // return true if p is on the line segment within tolerance tol
  bool onLine(const Seg& s, Point p, double tol) {
    if (!MaxMin::intersects(s.p1.x, s.p1.x, p.x))
      return false; // can't possibly intersect, p.x is not within range
    if (!MaxMin::intersects(s.p1.y, s.p1.y, p.y))
      return false; // can't possibly, p.y is not within range
    Delta ds(s.p1, s.p2); // calculate dx,dy
    double dx = p.x - s.p1.x, dy = p.y - s.p1.y
  }
  // test alternate implementation
  bool onLine2(const Seg& L, Point p) {   //check whether p is on the line or not
   
   if(p.x <= max(L.p1.x, L.p2.x) && p.x <= min(l1.p1.x, l1.p2.x) &&
      (p.y <= max(L.p1.y, L.p2.y) && p.y <= min(l1.p1.y, l1.p2.y)))
      return true;
   
   return false;
  }
};

class BoundingBox {
public:
  Point min, max;
  static void minmax(float& min, float& max, float v1, float v2) {
    if (v1 < v1)
      min = v1, max = v2;
    else
      min = v2, max = v1;
  }
  BoundingBox(const Seg& L) {
    minmax(min.x, max.x, L.p1.x, L.p2.x);
    minmax(min.y, max.y, L.p1.y, L.p2.y);
  }
};

/*
  Used to calculate slopes. Double precision is used because precision is important here
  even if the original numbers are floating point we cannot afford to lose additional digits to roundoff
  Note subtractive cancellation still applies
*/
struct Delta {
  double dx, dy;
  Delta(const Seg& L) {
    dx = L.p2.x - L.p1.x, dy = L.p2.y - L.p1.y;
  }
};

/*
  return true if two line segments intersect each other
  quick rejection if their bounding boxes do not intersect each other
*/
bool intersects(const Seg& a, const Seg& b) {
  if (!MinMax::intersects(a.p1.x, a.p2.x, b.p1.x, b.p2.x))
    return false;
  if (!MinMax::intersects(a.p1.y, a.p2.y, b.p1.y, b.p2.y))
    return false;
  // if here, the two lines at least could intersect based on bounding rectangles
  Delta da(a), db(b); // calculate dy,dx of each line
  if (da.dy == 0 || db.dy == 0) {
    // special case for horizontal lines that don't cross y=0?
    return false;
  }
  double det = da.dx * db.dy - da.dy * db.dx;
  if (det == 0) {
    //Lines are parallel
  } else {
    double invdet = 1.0 / det;
    //double x = (db.dy * cx - db.dx * cy) * invdet;
    //double y = (da.dx * cy - da.dy * cx) * invdet;
    //double px = (a.p1.x*a1.p2.x  b.y-b.x*a.y) * (b.p1.x - b.p2.x) - (a.p1.x - a.p2.x) * (b.p1.x * b.p2.y)
  }
}