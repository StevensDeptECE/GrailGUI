#pragma once

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

class Scale {
 protected:
  double scale;         // transform from data space to graph space (pixels)
  double shift;         // accounts for location of start of graph
  double min;           // minimum axis value
  double max;           // maximum axis value
  double tickInterval;  // distance between ticks in graph space
  double numInterval;   // TODO: distance between nums in graph space

 public:
  // Scale(double scale, double shift) : scale(scale), shift(shift) {}
  Scale() : scale(0), shift(0) {}
  virtual float transform(double x) = 0;
  virtual void init(double min, double max, double shift, double axisSize,
                    double tickInterval) = 0;
  virtual float next(float v) = 0;
};

class LinearScale : public Scale {
 public:
  // LinearScale(double scale, double shift) : Scale(scale, shift) {}
  float transform(double x) override;
  void init(double min, double max, double shift, double axisSize,
            double tickInterval) override;
  float next(float v) override;
};

class LogScale : public Scale {
 public:
  // LogarithmicScale(double scale, double shift) : Scale(scale, shift) {}
  float transform(double x) override;
  void init(double min, double max, double shift, double axisSize,
            double tickInterval) override;
  float next(float v) override;
};
