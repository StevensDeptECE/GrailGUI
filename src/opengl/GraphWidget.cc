#include "opengl/GraphWidget.hh"

#include <numbers>

#include "util/Ex.hh"

using namespace std;

GraphWidget::GraphWidget(Canvas* c, double x, double y, double w, double h,
                         uint32_t allowedXAxis, uint32_t allowedYAxis,
                         AxisType xAxisType, AxisType yAxisType, const GraphStyle* s)
    : SuperWidget2D(c, x, y, w, h),
      allowedXAxis(allowedXAxis),
      allowedYAxis(allowedYAxis),
      s(s)
 {
   xAxis = nullptr;
   createXAxis(xAxisType);
   yAxis = nullptr;
   createYAxis(yAxisType);
 }

GraphWidget::~GraphWidget() {
  // TODO: ccls no like this for some reason, claims that AxisWidget is abstract
  // but has non-virtual destructor
  delete xAxis;
  delete yAxis;
}

AxisWidget* GraphWidget::createAxis(uint32_t allowed, AxisType typ,
                                    const Style* axisStyle,
                                    const Style* axisTextStyle, bool isVert,
                                    AxisType& out) {
  if (!(typ & allowed)) throw Ex1(Errcode::BAD_ARGUMENT);

  StyledMultiShape2D* mnew;
  MultiText* tnew;
  double axisX, axisY, axisW, axisH;

  if (isVert) {
    mnew = new StyledMultiShape2D(c, &s->yAxisStyle, numbers::pi / 2, x - w, y + h);
    tnew = new MultiText(c, &s->yAxisTextStyle, 0, x, y);
    axisX = 0;
    axisY = 0;
    axisW = h;
    axisH = w;

  } else {
    mnew = new StyledMultiShape2D(c, axisStyle);
    tnew = new MultiText(c, axisTextStyle);
    axisX = x;
    axisY = y;
    axisW = w;
    axisH = h;
  }

  c->addLayer(mnew);
  c->addLayer(tnew);

  out = typ;

  switch (typ) {
    case LINEAR: {
      return new LinearAxisWidget(mnew, tnew, axisX, axisY, axisW, axisH);
    }; break;

    case LOGARITHMIC: {
      return new LogAxisWidget(mnew, tnew, axisX, axisY, axisW, axisH);
    }; break;

    case TEXT: {
      return new TextAxisWidget(mnew, tnew, axisX, axisY, axisW, axisH);
    }; break;

    default: {
      return nullptr;
    }; break;
  }
}

void GraphWidget::createXAxis(AxisType typ) {
  delete xAxis;
  xAxis = createAxis(allowedXAxis, typ, &s->xAxisStyle, &s->xAxisTextStyle, false,
                     xAxisType);
}

void GraphWidget::createYAxis(AxisType typ) {
  delete yAxis;
  yAxis = createAxis(allowedYAxis, typ, &s->yAxisStyle, &s->yAxisTextStyle, true,
                     yAxisType);
}

void GraphWidget::commonRender() {
  StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, &s->baseStyle));

  MultiText* t = c->addLayer(new MultiText(c, &s->baseStyle));
  const Font* titleFont = s->titleStyle.f;
  if (graphTitle.size())
    t->addCentered(x + w / 2, y - titleFont->getHeight(),
                   titleFont, graphTitle.c_str(), graphTitle.size());

  m->drawLine(x, y, x + w, y, grail::black);
  m->drawLine(x + w, y, x + w, y + h, grail::black);
  xAxis->init();
  yAxis->init();
}

void GraphWidget::setGraphTitle(const std::string& text) { graphTitle = text; }

void GraphWidget::setStyle(const GraphStyle* s) {
	xAxis->setTickDrawSize(s->xTickDrawSize);
	yAxis->setTickDrawSize(s->yTickDrawSize);
	xAxis->setAxisColor(grail::green);
	xAxis->setTickColor(grail::purple);
	yAxis->setIsVert(true);
	yAxis->setShowTicks(true);
	yAxis->setAxisColor(grail::yellow);
  yAxis->setTickColor(grail::red);
}
