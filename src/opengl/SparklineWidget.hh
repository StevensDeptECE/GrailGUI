#pragma once
#include <memory>
#include "opengl/Widget2D.hh"
#include "opengl/Scale.hh"
#include <string>
#include <vector>
#include <algorithm>

class Style;
class SparklineWidget : public Widget2D {
private:
  std::string title;
  const Style* titleStyle;
  const Style* barStyle;
  std::unique_ptr<Scale> yAxisScale;
  std::unique_ptr<Scale> xAxisScale;
  float relativeSpace; //space between bars as a fraction of bar width 
  float minX;
  float minY;
  float maxX;
  float maxY;
  float maxMultiplier; 
  float tickSize;
  float tickStart;
  Scale *yAxis;
  Scale *xAxis;
  float minMultiplier;

  Canvas ca;

  int count;
  glm::vec4 lineColor;
  std::vector <double> anim;
  int nPerBox;
  int smoothness;
  std::vector <double> xVec;
  std::vector <double> yVec;

public:
  SparklineWidget(StyledMultiShape2D* m, MultiText* t,
   float x, float y, float w, float h,
   const std::string& title, const Style *titleStyle,
   const Style *barStyle, 
   float minX, float maxX, float minY, float maxY , float maxMultiplier, float minMultiplier,
   float tickSize, float tickStart, Scale *yAxis, Scale *xAxis) : 
    Widget2D(m, t, x, y, w, h), title(title), titleStyle(titleStyle), 
    barStyle(barStyle), minX(minX), maxX(maxX), minY(minY), maxY(maxY), 
    maxMultiplier(maxMultiplier), minMultiplier(minMultiplier),
    tickSize(tickSize), tickStart(tickStart), yAxis(yAxis), xAxis(xAxis){
      glViewport(x, y, w, h);
    }

  SparklineWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) :
    SparklineWidget(m,t, x, y, w, h, std::string (""), nullptr, nullptr,
    0, 100, 0, 100, 1.2, 10, 10, x, new LinearScale(), new LinearScale()){} 


  SparklineWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h, 
  const std::vector<double>& xLocations, const std::vector<double>& yLocations) :
    SparklineWidget(m,t, x, y, w, h, "", nullptr, nullptr,
    0, 0, 0, 0, 1.2, 10, 10, x, new LinearScale(), new LinearScale()){
      maxY = maxMultiplier*(*max_element(yLocations.begin(), yLocations.end()));
      maxX = (*max_element(xLocations.begin(), xLocations.end()));
    }  


  void setMinMaxY(float min, float max){
    this->minY = min;
    this->maxY = max;
  }
  void setAxisScale(Scale *yAxis){this->yAxis = yAxis;}
  void setTitleStyle(const Style* s) { titleStyle = s;}
  void chart(const std::vector<double>& yLocations, const std::vector<double>& xLocations,
  glm::vec4& c);
  //void chartLog(const float b[], int size, float relativeSpace, const std::string barNames[], int logBase);
  void setTitle(const std::string& s);
  void init() override;
  void setAnimation(const std::vector<double>& yLocations, int nPerBox, int smoothness, glm::vec4& c);
  void update();
};