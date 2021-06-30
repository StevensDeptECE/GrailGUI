#include "opengl/GraphWidget.hh"

using namespace std;

void GraphWidget::commonRender() {
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
