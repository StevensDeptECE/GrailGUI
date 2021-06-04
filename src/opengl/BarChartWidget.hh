#pragma once

#include "opengl/Widget2D.hh"
#include <string>
#include <vector>

class Style;
class BarChartWidget : public Widget2D {
private:
  std::string text;
  const Style* titleStyle;
  const Style* barStyle;

public:
  BarChartWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) : 
    Widget2D(m, t, x, y, w, h), text(), titleStyle(nullptr) {}
  void setTitleStyle(const Style* s) { titleStyle = s;}
  void chart(const std::vector<float>& b, float space, 
    const std::vector<std::string>& barNames, int rullerInterval);
  void chartLog(const float b[], int size, float relativeSpace, const std::string barNames[], int logBase);
  void title(const std::string& s);
  void init() override;
};