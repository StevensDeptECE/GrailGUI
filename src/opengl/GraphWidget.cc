#include "opengl/GraphWidget.hh"

using namespace std;

GraphWidget::GraphWidget(Canvas *c, double x, double y, double w, double h,
                         Style *baseStyle, Style *xAxisStyle,
                         Style *xAxisTextStyle, Style *yAxisStyle,
                         Style *yAxisTextStyle)
    : SuperWidget2D(c, x, y, w, h),
      baseStyle(baseStyle),
      xAxisTextStyle(xAxisStyle),
      xAxisStyle(xAxisTextStyle),
      yAxisTextStyle(yAxisStyle),
      yAxisStyle(yAxisTextStyle),
      xAxis(nullptr),
      yAxis(nullptr) {}

GraphWidget::~GraphWidget() {
  // TODO: ccls no like this for some reason, claims that AxisWidget is abstract
  // but has non-virtual destructor
  delete xAxis;
  delete yAxis;
}

void GraphWidget::setBaseStyle(const Style *s) { baseStyle = s; }

void GraphWidget::commonRender() {
  StyledMultiShape2D *m = c->addLayer(new StyledMultiShape2D(c, baseStyle));

  MultiText *t = c->addLayer(new MultiText(c, baseStyle));
  if (graphTitle.size())
    t->addCentered(x + w / 2, y - m->getStyle()->f->getHeight(),
                   m->getStyle()->f, graphTitle.c_str(), graphTitle.size());

  m->drawLine(x, y, x + w, y, grail::black);
  m->drawLine(x + w, y, x + w, y + h, grail::black);
  xAxis->init();
  yAxis->init();
}

void GraphWidget::setGraphTitle(std::string text) { graphTitle = text; }

void GraphWidget::setXAxisTextStyle(const Style *xAxisTextStyle) {
  this->xAxisTextStyle = xAxisTextStyle;
}

void GraphWidget::setYAxisTextStyle(const Style *yAxisTextStyle) {
  this->yAxisTextStyle = yAxisTextStyle;
}

void GraphWidget::setXAxisStyle(const Style *xAxisStyle) {
  this->xAxisStyle = xAxisStyle;
}

void GraphWidget::setYAxisStyle(const Style *yAxisStyle) {
  this->yAxisStyle = yAxisStyle;
}
