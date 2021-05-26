#pragma once

#include "opengl/Widget2D.hh"
#include <string>

class Style;
class BarChartWidget : public Widget2D {
private:
  std::string text;
  const Style* titleStyle;
  const Style* barStyle;

public:
  BarChartWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) : Widget2D(m,t, x, y, w, h) {}
  void setTitleStyle(const Style* s) { titleStyle = s; }
  void setBarStyle( const Style* s) { barStyle = s;  }
  void chart(double x[], double y[], int size){
    
  }
  void init() override;
};