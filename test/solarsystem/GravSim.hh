
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "opengl/GrailGUI.hh"

class Vector3D {
 public:
  double x;
  double y;
  double z;

  Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  friend std::ostream& operator<<(std::ostream& s, const Vector3D& vec) {
    return s << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  }

  friend Vector3D operator+(Vector3D& left, Vector3D& right) {
    return Vector3D(left.x + right.x, left.y + right.y, left.z + right.z);
  }

  Vector3D& operator+(const Vector3D& a) {
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
  }

  friend Vector3D operator-(Vector3D& left, Vector3D& right) {
    return Vector3D(left.x - right.x, left.y - right.y, left.z - right.z);
  }

  friend Vector3D operator*(Vector3D& left, Vector3D& right) {
    return Vector3D(left.x * right.x, left.y * right.y, left.z * right.z);
  }
  friend Vector3D operator/(Vector3D& left, Vector3D& right) {
    return Vector3D(left.x / right.x, left.y / right.y, left.z / right.z);
  }

  friend Vector3D operator*(double val, Vector3D& vec) {
    return Vector3D(val * vec.x, val * vec.y, val * vec.z);
  }

  Vector3D& operator*(const double val) {
    x *= val;
    y *= val;
    z *= val;
    return *this;
  }

  friend Vector3D operator/(double val, Vector3D& vec) {
    return Vector3D(val / vec.x, val / vec.y, val / vec.z);
  }

  friend Vector3D operator/(Vector3D& vec, double val) {
    return Vector3D(val / vec.x, val / vec.y, val / vec.z);
  }

  double dot(Vector3D& vec) const { return x * vec.x + y * vec.y + z * vec.z; }
  double mag() const { return sqrt(x * x + y * y + z * z); }

  double dist(Vector3D& vec) const {
    return sqrt(pow(vec.x - x, 2) + pow(vec.y - y, 2) + pow(vec.z - z, 2));
  }
  Vector3D normalize() const {
    return Vector3D(this->x / this->mag(), this->y / this->mag(),
                    this->z / this->mag());
  }
  Vector3D perpendicular() const { return Vector3D(-y, x, z); }

  Vector3D cross(Vector3D& vec) const {
    return Vector3D((y * vec.z) - (z * vec.y), (z * vec.x) - (x * vec.z),
                    (x * vec.y) - (y * vec.x));
  }
  double anglebetween(Vector3D& vec) const {
    return acos((this->dot(vec)) / (this->mag() * vec.mag()));
  }
};

class JulDate {
 private:
  static int EPOCH;

  double jday;

  JulDate(double jd) { jday = jd; }

 public:
  JulDate(int year, int mon, int day, int hour, int min, int second) {
    time_t theTime;
    time(&theTime);
    struct tm* aTime = localtime(&theTime);
    jday = ((1461 * (year + 4800 + (mon - 14) / 12)) / 4 +
            (367 * (mon - 2 - 12 * ((mon - 14) / 12))) / 12 -
            (3 * ((year + 4900 + (year - 14) / 12) / 100)) / 4 + day - 32075) +
           (((hour + 12.0) / 24)) + (min / 1440.0) + (second / 86400.0);
  }

  JulDate() {
    time_t theTime;
    time(&theTime);
    struct tm* aTime = localtime(&theTime);
    int year = aTime->tm_year + 1900;
    int mon = aTime->tm_mon + 1;
    int day = aTime->tm_mday;
    int hour = aTime->tm_hour;
    int min = aTime->tm_min;
    int second = aTime->tm_sec;
    jday = ((1461 * (year + 4800 + (mon - 14) / 12)) / 4 +
            (367 * (mon - 2 - 12 * ((mon - 14) / 12))) / 12 -
            (3 * ((year + 4900 + (year - 14) / 12) / 100)) / 4 + day - 32075) +
           (((hour + 12.0) / 24)) + (min / 1440.0) + (second / 86400.0);
  }

  double getJDay() const { return jday; }

  friend double operator-(JulDate a, JulDate b) { return a.jday - b.jday; }

  friend JulDate operator+(JulDate left, int a) {
    return JulDate(left.jday + a);
  }
};

class Body {
 private:
  Vector3D position;
  Vector3D velocity;
  Vector3D acceleration;

  std::string name;
  std::string orbits;

  double mass;
  double diameter;
  double Perhelion;
  double Aphelion;
  double distFromSun;
  double trueAnomly;

  double a;  // Semi Major Axis
  double e;  // orbital eccentricty
  double i;  // orbital inclination
  double o;  // longtiude of ascedning node
  double p;  // longitdeu of perhelion
  double L;  // mean longitude
  double n;
  double m;

  // deg/day

 public:
  Body()
      : name(),
        orbits(),
        mass(),
        diameter(),
        Perhelion(),
        Aphelion(),
        position(),
        velocity(),
        acceleration(),
        distFromSun(),
        trueAnomly(),
        a(),
        e(),
        i(),
        o(),
        p(),
        L(),
        n(),
        m() {}

  Vector3D getPosition() const { return position; }
  Vector3D getVelocity() const { return position; }
  Vector3D getAcceleration() const { return acceleration; }
  std::string getName() const { return name; }
  double getMass() const { return mass; }
  double getDiameter() const { return diameter; }
  double getDistFromSun() const { return distFromSun; }

  friend std::ostream& operator<<(std::ostream& s, const Body b) {
    return s << b.name << ": " << b.distFromSun << "au " << b.position << " "
             << b.acceleration << " " << b.velocity << " " << std::endl;
  }

  friend std::istream& operator>>(std::istream& s, Body* b) {
    s >> b->name >> b->orbits >> b->a >> b->e >> b->i >> b->o >> b->p >> b->L >>
        b->n >> b->m >> b->diameter;
    return s;
  }

  static void setPosition(std::vector<Body*> bodies, JulDate t) {
    double dayNumber = t.getJDay() - 2451545.00000;

    for (auto b : bodies) {
      if (b->name != "Sun") {
        double M = b->m + b->n * dayNumber;
        if (M < 0) {
          while (M < 0) {
            M = M + 360;
          }
        }
        if (M > 360) {
          while (M > 360) {
            M = M - 360;
          }
        }

        double E = M + (180 / M_PI) * (b->e) * (sin(M * M_PI / 180)) *
                           (1 + (b->e) * (cos(M * M_PI / 180)));

        double x = (b->a) * (cos(E * M_PI / 180) - (b->e));

        double y = (b->a) * sqrt(1 - (b->e * b->e)) * sin(E * M_PI / 180);

        b->distFromSun = sqrt(x * x + y * y);

        b->trueAnomly = atan2(y, x);

        double xh = b->distFromSun *
                    (cos(b->o) * cos(b->trueAnomly + b->p - b->o) -
                     sin(b->o) * sin(b->trueAnomly + b->p - b->o) * cos(b->i));

        double yh = b->distFromSun *
                    (sin(b->o) * cos(b->trueAnomly + b->p - b->o) +
                     cos(b->o) * sin(b->trueAnomly + b->p - b->o) * cos(b->i));

        double zh =
            b->distFromSun * (sin(b->trueAnomly + b->p - b->o) * sin(b->i));

        b->position.x = xh;
        b->position.y = yh;
        b->position.z = zh;
      }
    }
  }

  static void setVelocity(std::vector<Body*> bodies, JulDate t) {
    for (auto b : bodies) {
      if (b->name != "Sun") {
        double mew = 1.32712440042e20;
        double tempA = 0;
        tempA = b->a * 1.496e+11;
        double h = pow(mew * (tempA) * (1 - pow((b->e), 2)), 0.5);

        double Vx =
            ((-mew / h) *
             (cos(b->o) *
                  (sin(b->trueAnomly + b->p - b->o) + (b->e) * sin(b->o)) +
              sin(b->o) *
                  (cos(b->trueAnomly + b->p - b->o) + (b->e) * cos(b->o)) *
                  cos(b->i))) /
            1000;

        double Vy =
            ((-mew / h) *
             (sin(b->o) *
                  (sin(b->trueAnomly + b->p - b->o) + (b->e) * sin(b->o)) -
              cos(b->o) *
                  (cos(b->trueAnomly + b->p - b->o) + (b->e) * cos(b->o)) *
                  cos(b->i))) /
            1000;

        double Vz =
            ((mew / h) * ((cos(b->trueAnomly + b->p - b->o)) * sin(b->i))) /
            1000;

        b->velocity.x = Vx;
        b->velocity.y = Vy;
        b->velocity.z = Vz;
      }
    }
  }
  static void computeAccelerations(std::vector<Body*>& bodies, double dt) {
    for (auto b : bodies) {
      Vector3D a = 0;

      for (auto b2 : bodies) {
        if (b == b2) {
          continue;
        }
        //  Gm/rsq
        const double G = 6.67E-11;
        Vector3D dPos = b2->position - b->position;
        double r = dPos.mag();

        double aMag = G * b2->mass / (r * r);
        a = a +
            Vector3D(aMag * dPos.x / r, aMag * dPos.y / r, aMag * dPos.z / r);
      }

      b->acceleration = a;
    }
  }
  static void stepForward(std::vector<Body*>& bodies, double dt) {
    for (auto b : bodies) {
      b->position =
          b->position + b->velocity * dt + 0.5 * b->acceleration * dt * dt;
      b->velocity = b->velocity + b->acceleration * dt;
    }
  }
};

class NASAEphemerisSolarSystem {
 public:
  std::vector<Body*> bodies;
  JulDate currentTime;

 public:
  NASAEphemerisSolarSystem(const char* filename) : currentTime() {
    std::ifstream f(filename);

    char buf[1024];
    f.getline(buf, sizeof(buf));

    Body* b;

    while (f.getline(buf, sizeof(buf))) {
      b = new Body;
      std::istringstream line(buf);
      line >> b;
      bodies.push_back(b);
    }

    //    Body::setInitialConditions(bodies, currentTime);
    Body::setPosition(bodies, currentTime);
    Body::setVelocity(bodies, currentTime);
  }

  virtual void advanceTime(double dt) {
    this->currentTime = this->currentTime + dt;
    Body::setPosition(bodies, currentTime);
    Body::setVelocity(bodies, currentTime);
  }

  ~NASAEphemerisSolarSystem() {
    for (auto b : bodies) {
      delete b;
    }
  }

  friend std::ostream& operator<<(std::ostream& s,
                                  const NASAEphemerisSolarSystem& Sol) {
    for (auto b : Sol.bodies) {
      s << " " << *b << " ";
    }
    return s;
  }
};

class GravSimSolarSystem : public NASAEphemerisSolarSystem {
 public:
  void advanceTime(double dt) override {
    Body::computeAccelerations(bodies, dt);
    Body::stepForward(bodies, dt);  // pos = pos + v*dt + 0.5*a*dt*dt
  }
};