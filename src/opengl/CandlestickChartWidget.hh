#pragma once
#include <memory>
#include "opengl/Widget2D.hh"
#include "opengl/Scale.hh"
#include <string>
#include <vector>
#include <algorithm>

class Style;
class CandlestickChartWidget : public Widget2D {
private:
  std::string title;
  const Style* titleStyle;
  const Style* barStyle;
  std::unique_ptr<Scale> yAxisScale;
  float relativeSpace; //space between bars as a fraction of bar width 
  float min; //graph bottom, in graph units
  float max;
  float maxMultiplier; 
  float tickSize;
  float tickStart;
  Scale *yAxis;
  int tickThicknessStart;
  int tickThicknessEnd;

public:
  CandlestickChartWidget(StyledMultiShape2D* m, MultiText* t,
   float x, float y, float w, float h,
   const std::string& title, const Style *titleStyle,
   const Style *barStyle, 
   float min, float max, float maxMultiplier, float relativeSpace, 
   int tickThicknessStart, int tickThicknessEnd,
   float tickSize, float tickStart, Scale *yAxis) : 
    Widget2D(m, t, x, y, w, h), title(title), titleStyle(titleStyle), 
    barStyle(barStyle), min(min), max(max), maxMultiplier(maxMultiplier), relativeSpace(relativeSpace),
    tickThicknessStart(tickThicknessStart), tickThicknessEnd(tickThicknessEnd),
    tickSize(tickSize), tickStart(tickStart), yAxis(yAxis) {}

  CandlestickChartWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) :
    CandlestickChartWidget(m,t, x, y, w, h, std::string (""), nullptr, nullptr,
    0, 100, 1.25, 1.25, 1, 1, 10, x, new LinearScale()){} 


  CandlestickChartWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h, 
  const std::vector<float>& b) :
    CandlestickChartWidget(m,t, x, y, w, h, "", nullptr, nullptr,
    0, 0, 1.25, 1.25, 1, 1, 10, x, new LinearScale()){
      max = maxMultiplier*(*max_element(b.begin(), b.end()));
    }  


  void setMinMax(float min, float max){
    this->min = min;
    this->max = max;
  }
  void setAxisScale(Scale *yAxis){this->yAxis = yAxis;}
  void setTitleStyle(const Style* s) { titleStyle = s;}
  void chart(const std::vector<float>& b, int rulerInterval, int dataPointsPerBar);
  //void chartLog(const float b[], int size, float relativeSpace, const std::string barNames[], int logBase);
  void setTitle(const std::string& s);
  void init() override;
};