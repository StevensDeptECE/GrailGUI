#pragma once

#include "opengl/GLMath.hh"
#include "opengl/Shape2D.hh"

class Sector : public Shape2D {
 private:
  float xRad, yRad, fromAngle, toAngle, angleInc;

 public:
  Sector(Canvas* c, float x, float y, float xRad, float yRad, float fromAngle,
         float toAngle, float angleInc, Style* s)
      : Shape2D(c, x, y, s),
        xRad(xRad),
        yRad(yRad),
        fromAngle(fromAngle),
        toAngle(toAngle),
        angleInc(angleInc) {
    addPoint(x, y);

    for (float i = fromAngle; i <= toAngle; i += angleInc) {
      float px = this->x + xRad * cos(i * DEG2RAD<float>);
      float py = this->y + yRad * sin(i * DEG2RAD<float>);

      addPoint(px, py);
    }

    initIndices();
  }

  void initIndices();
  void render();
};
