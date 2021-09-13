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

AxisWidget* GraphWidget::createAxis(uint32_t allowed, AxisType axisType,
                                    const Style* axisStyle,
                                    const Style* axisTextStyle, bool isVert,
																		AxisType& out) {
  if (!(axisType & allowed)) throw Ex1(Errcode::BAD_ARGUMENT);

  StyledMultiShape2D* mnew;
  double axisX, axisY, axisW, axisH;

  if (isVert) {
    mnew = new StyledMultiShape2D(c, axisStyle, numbers::pi / 2, x - w, y + h);
    axisX = 0;
    axisY = 0;
    axisW = h;
    axisH = w;
  } else {
    mnew = new StyledMultiShape2D(c, axisStyle);
    axisX = x;
    axisY = y;
    axisW = w;
    axisH = h;
  }
  MultiText* tnew = new MultiText(c, axisTextStyle, 0, x, y);
  c->addLayer(mnew);
  c->addLayer(tnew);

  out = axisType;

  switch (axisType) {
    case LINEAR:
      return new LinearAxisWidget(mnew, tnew, axisStyle, axisTextStyle, axisX, axisY, axisW, axisH, isVert);
    case LOGARITHMIC:
      return new LogAxisWidget(mnew, tnew, axisStyle, axisTextStyle, axisX, axisY, axisW, axisH, isVert);
    case TEXT:
      return new TextAxisWidget(mnew, tnew, axisStyle, axisTextStyle, axisX, axisY, axisW, axisH, isVert);

			//TODO: eventually support axis type where each value gets the same space, and clustered
			// where somehow groups of different values are scaled to fit
    default:
			throw Ex1(Errcode::BAD_ARGUMENT); // unsupported axis type
  }
}

void GraphWidget::createXAxis(AxisType axisType) {
  delete xAxis;
  xAxis = createAxis(allowedXAxis, axisType, &s->xAxisStyle, &s->xAxisTextStyle, false,
                     xAxisType);
}

void GraphWidget::createYAxis(AxisType typ) {
  delete yAxis;
  yAxis = createAxis(allowedYAxis, typ, &s->yAxisStyle, &s->yAxisTextStyle, true,
                     yAxisType);
	yAxis->setIsVert(true);
}

void GraphWidget::commonRender() {
  m = c->addLayer(new StyledMultiShape2D(c, &s->borderStyle));
  t = c->addLayer(new MultiText(c, &s->borderStyle));
	const Font* titleFont = s->titleStyle.f;
  if (title.size())
    t->addCentered(x + w / 2, y - titleFont->getHeight(), titleFont, title);

	if (s->drawBorder) {
		m->drawRectangle(x, y, w, h, s->borderStyle.fg);
	}
	if (s->drawAxes) {
		xAxis->init();
		yAxis->init();
	}
}

void GraphWidget::setTitle(const std::string& text) { title = text; }

void GraphWidget::setStyle(const GraphStyle* s) {
	xAxis->setTickDrawSize(s->xTickSize);
	yAxis->setTickDrawSize(s->yTickSize);
	xAxis->setAxisColor(s->xAxisTextStyle.fg);
	xAxis->setTickColor(s->xAxisStyle.fg);
	yAxis->setIsVert(true);
	yAxis->setShowTicks(s->drawTicks);
	yAxis->setAxisColor(s->yAxisTextStyle.fg);
  yAxis->setTickColor(s->yAxisStyle.fg);
}
