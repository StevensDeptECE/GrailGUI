#pragma once

#include <memory>
#include <vector>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/Style.hh"

typedef double (*FuncOneVar)(double x);

class Tab;
class PointPlot {
 private:
  MultiShape2D *m;
  Canvas *c;
  Style *s;
  Style *textStyle;
  std::string title;
  std::string func;
  double x_start;
  double x_end;
  double x_step;
  uint32_t pixXMin;
  uint32_t pixXMax;
  uint32_t numPoints;
  std::vector<float> X, Y;
  uint32_t cWidth, cHeight;

 public:
  PointPlot(std::shared_ptr<Tab> tabs, float x, float y, float w, float h);
  void add_Point(float x, float y);
  // void remove_point(float x, float y);
  void add_Points(std::vector<float> &x, std::vector<float> &y);
  void linePlot(std::vector<float> &X, std::vector<float> &Y);
  void scatterPlot(std::vector<float> &X, std::vector<float> &Y);
  void linearRegression(std::vector<float> &X, std::vector<float> &Y);
  float maxWidth(std::vector<float> &X);
  float minWidth(std::vector<float> &X);
  float maxHeight(std::vector<float> &Y);
  float minHeight(std::vector<float> &Y);
  void drawAxes();
  void drawTicks();
  void tickValues(std::vector<float> &X, std::vector<float> &Y);
  void chartTitle(std::string title);
  // void drawSin(double x_start,double x_end, double x_step);
  void drawFunc(double x_min, double x_max, double y_min, double y_max,
                FuncOneVar f, uint32_t pixXMin, uint32_t pixXMax,
                uint32_t pixYMin, uint32_t pixYMax, uint32_t numPoints);
  void drawTwoFunc(double val_min, double val_max, double step, FuncOneVar f,
                   FuncOneVar g, int scaleF, int scaleG);
};
