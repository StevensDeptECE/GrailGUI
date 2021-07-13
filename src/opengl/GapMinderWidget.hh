#pragma once
#include <memory>
#include "opengl/Widget2D.hh"
#include "opengl/Scale.hh"
#include <string>
#include <vector>
#include <algorithm>
#include "data/GapMinderLoader.hh"

class Style;
class GapMinderWidget : public Widget2D {
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
  int rulerIntervalX;
  int rulerIntervalY;

  GapMinderLoader *gml;
  const GapMinderLoader::Dataset* d;
  const GapMinderLoader::Dataset* d2;
  const GapMinderLoader::Dataset* d3;
  int startYear;
  int endYear;

public:
  GapMinderWidget(StyledMultiShape2D* m, MultiText* t,
   float x, float y, float w, float h,
   const std::string& title, const Style *titleStyle,
   const Style *barStyle, 
   float minX, float maxX, float minY, float maxY , float maxMultiplier, float minMultiplier,
   float tickSize, float tickStart, Scale *yAxis, Scale *xAxis) : 
    Widget2D(m, t, x, y, w, h), title(title), titleStyle(titleStyle), 
    barStyle(barStyle), minX(minX), maxX(maxX), minY(minY), maxY(maxY), 
    maxMultiplier(maxMultiplier), minMultiplier(minMultiplier),
    tickSize(tickSize), tickStart(tickStart), yAxis(yAxis), xAxis(xAxis), gml(nullptr){}

  GapMinderWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) :
    GapMinderWidget(m,t, x, y, w, h, std::string (""), nullptr, nullptr,
    0, 100, 0, 100, 1.25, 10, 10, x, new LinearScale(), new LinearScale()){} 


  GapMinderWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h, 
  const std::vector<float>& xLocations, const std::vector<float>& yLocations) :
    GapMinderWidget(m,t, x, y, w, h, "", nullptr, nullptr,
    0, 0, 0, 0, 1.25, 10, 10, x, new LinearScale(), new LinearScale()){
      maxY = maxMultiplier*(*max_element(yLocations.begin(), yLocations.end()));
      //for (int i = 0; i < yLocations.size(); i++){
        //std::vector <float>::const_iterator start = yLocations.begin()+i;
        //std::vector <float>::const_iterator end = yLocations.end();
        //std::cout << *max_element(start, end) << "\n";
      //}
      maxX = maxMultiplier*(*max_element(xLocations.begin(), xLocations.end()));
    }  


  void setMinMaxY(float min, float max){
    this->minY = min;
    this->maxY = max;
  }
  void setAxisScale(Scale *yAxis){this->yAxis = yAxis;}
  void setTitleStyle(const Style* s) { titleStyle = s;}
  void chart(const std::vector<float>& yLocations, const std::vector<float>& xLocations, 
  const std::vector<float>& sizes, int rulerIntervalX, int rulerIntervalY, const std::vector <glm::vec4>& c);
  void loadData(const std::string &sy, const std::string &sx, const std::string &ss, 
  int startYear, int endYear, int rulerIntervalX, int rulerIntervalY);
  void update();
  //void chartLog(const float b[], int size, float relativeSpace, const std::string barNames[], int logBase);
  void setTitle(const std::string& s);
  void init() override;
};