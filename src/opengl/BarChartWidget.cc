#include "opengl/BarChartWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"

using namespace std;

void BarChartWidget::init() {
  m ->drawRectangle(x, y, w, h, grail::black);
  m->fillCircle(x, y, w, 5, grail::black);
}