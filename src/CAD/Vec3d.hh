#pragma once
#include <cmath>
#include <iostream>

// create constructor to create the vec_3d objects
class Vec3D {
 public:
  double x;
  double y;
  double z;
  Vec3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  // overload operators for vector math
  friend Vec3D operator+(const Vec3D& a, const Vec3D& b) {
    return Vec3D(a.x + b.x, a.y + b.y, a.z + b.z);
  }
  friend Vec3D operator-(const Vec3D& a, const Vec3D& b) {
    return Vec3D(a.x - b.x, a.y - b.y, a.z - b.z);
  }
  friend Vec3D operator*(const Vec3D& a, double s) {
    return Vec3D(a.x * s, a.y * s, a.z * s);
  }
  friend Vec3D operator/(const Vec3D& a, double s) {
    return Vec3D(a.x / s, a.y / s, a.z / s);
  }

  Vec3D operator-() const { return Vec3D(-x, -y, -z); }
  // create three methods to compute various vector properties
  double mag() const { return sqrt(magsq()); }
  double magsq() const { return x * x + y * y + z * z; }
  Vec3D normalize(const Vec3D& a) {
    if (a.mag() == 0) {
      return Vec3D(0, 0, 0);
    } else {
      return a / a.mag();
    }
  }

  // compute the dot product as a function
  friend double dot(const Vec3D& a, const Vec3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }
  friend Vec3D cross(const Vec3D& a, const Vec3D& b) {
    double cx = (a.y * b.z) - (a.z * b.y);
    double cy = (a.z * b.x) - (a.x * b.z);
    double cz = (a.x * b.y) - (a.y * b.x);
    return Vec3D(cx, cy, cz);
  }

  friend double distance(const Vec3D& a, const Vec3D& b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
  }

  friend Vec3D midpoint(const Vec3D& a, const Vec3D& b) {
    return Vec3D(((a.x + b.x) / 2), ((a.y + b.y) / 2), ((a.z + b.z) / 2));
  }

  // Overload the << operator
  friend std::ostream& operator<<(std::ostream& s, const Vec3D& v) {
    return s << "(" << v.x << "," << v.y << "," << v.z << ")";
  }
  // Overload the >> operator
  friend std::istream& operator>>(std::istream& s, Vec3D& v) {
    return s >> v.x >> v.y >> v.z;
  }
};

// int main() {
// const Vec3D a(1,2,3);
// const Vec3D b(1,2,3);
// cout << a.magsq();
// cout << a << b;
// }
