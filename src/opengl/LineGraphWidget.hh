#pragma once

#include <memory>
#include <string>
#include <vector>

#include "opengl/Errcode.hh"
#include "opengl/Scale.hh"
#include "opengl/Widget2D.hh"
#include "util/Ex.hh"

class Style;
class LineGraphWidget : public Widget2D {
 private:
  std::string text;
  const Style *titleStyle;
  const Style *barStyle;
  std::unique_ptr<Scale> xAxisScale;
  std::unique_ptr<Scale> yAxisScale;
  std::vector<glm::vec4> colors;
  std::vector<std::string> names;
  int startPoint, endPoint;
  Scale *yAxis;
  Scale *xAxis;


 public:
  LineGraphWidget(StyledMultiShape2D *m, MultiText *t, float x, float y,
                  float w, float h)
      : Widget2D(m, t, x, y, w, h), text(), titleStyle(nullptr) {}
  void setTitleStyle(const Style *s) { titleStyle = s; }
  void chart(float xMax, float yMax, float xInterval, float yInterval,
                            Scale *xAxis, Scale *yAxis, int start, int end);
  // void chartLog(const float b[], int size, float relativeSpace,
  //             const std::string barNames[], int logBase);
  void add(const std::vector<float> &xPoints,
                            const std::vector<float> &yPoints,
                            const glm::vec4& rgb, std::string name);
  void title(const std::string &s);
  void axes(char xLabel[], int xLen, char yLabel[], int yLen, float xMax, float yMax, float xMin, float yMin, float xInterval, float yInterval);
  void axes(char xLabel[], int xLen, char yLabel[], int yLen, float xMax, float yMax, float xMin, float yMin, float xInterval, float yInterval, int start, int end);
  void legend(float x, float y);
  void init() override;
};
