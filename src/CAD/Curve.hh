#include "opengl/GrailGUI.hh"
#include "opengl/util/Transformation.hh"


class Curve {
  protected:
    typedef std::array<double,3> Vector;
    uint32_t used;
    uint32_t size;
    std::vector<Vector> points;
  
  public:
    Curve() {
      used = 0;
      size = 2;
    }

  void add(Vector p);
  void resize();
  uint32_t size();
  virtual Vector getPoint(double step);
  virtual Vector getTangent(double step); //virtual replaces "abstract" from java
  Transformation getMatrix(double step); //implement Transformation/import for now but function/constructor is different
};