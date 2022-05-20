#pragma once

#include "xdl/std.hh"

class StyledMultiShape2D;
class MultiText;

class Calendar {
 private:
  StyledMultiShape2D* sms;
  MultiText* mt;
  float x0, y0, w, h;
  glm::vec4 edgeColor;
  Date start;

 public:
  Calendar(StyledMultiShape2D* sms, MultiText* mt, float x0, float y0, float w,
           float h)
      : sms(sms), mt(mt), edgeColor(0, 0, 0, 1) {}
  void setDate(Date start) { this->start = start; }
  void drawWeek() const;
  void drawMonth() const;
  void drawYear() const;
  void draw() const;
};
