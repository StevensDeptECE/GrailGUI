#include "opengl/GraphWidget.hh"

using namespace std;

void GraphWidget::setGraphTitle(std::string text) { graphTitle = text; }

void GraphWidget::setXAxisTextStyle(const Style *xAxisTextStyle) {
  this->xAxisTextStyle = xAxisTextStyle;
}

void GraphWidget::setYAxisTextStyle(const Style *yAxisTextStyle) {
  this->yAxisTextStyle = yAxisTextStyle;
}
