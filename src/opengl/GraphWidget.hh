#pragma once

#include "opengl/Widget2D.hh"
#include <string>

class Style;
class GraphWidget : public Widget2D {
private:
  std::string text;
  const Style* titleStyle;
  const Style* lineStyle;
public:
  GraphWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) : Widget2D(m,t, x, y, w, h) {}
  void setTitleStyle(const Style* s) { titleStyle = s; }
  void setLineStyle( const Style* s) { lineStyle = s;  }
  void init() override;
};