#pragma once
#include "opengl/GraphWidget.hh"
#include "opengl/Scale.hh"
#include <string>
#include <vector>
//#include <algorithm>
#include "data/GapMinderLoader.hh"
#include "opengl/Colors.hh"

class GraphStyle;
class GapMinderWidget : public GraphWidget {
private:
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

  GapMinderLoader *gml;
  const GapMinderLoader::Dataset* d;
  const GapMinderLoader::Dataset* d2;
  const GapMinderLoader::Dataset* d3;

	//TODO: remove vectors and make this passive pointers with stride to underlying blocks of float
	// for efficiency
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> sizes;
	std::vector<float> colorValues;
	
  int startYear;
  int endYear;

public:
	//TODO: 3 axes defined as numeric, but the 4th should map to the color of the circles
  GapMinderWidget(Canvas* c, float x, float y, float w, float h,
									AxisType xAxisType, AxisType yAxisType, AxisType sizeAxisType, const GraphStyle* s)
		: GraphWidget(c, x, y, w, h, FUNCTIONS_PERMITTED, FUNCTIONS_PERMITTED, xAxisType, yAxisType, s)

	{}
			
	//   float minX, float maxX, float minY, float maxY , float maxMultiplier, float minMultiplier,
	//    Scale *yAxis, Scale *xAxis) : 
	//    Widget2D(m, t, x, y, w, h), title(title), titleStyle(titleStyle), 
	//    barStyle(barStyle), minX(minX), maxX(maxX), minY(minY), maxY(maxY), 
	//    maxMultiplier(maxMultiplier), minMultiplier(minMultiplier),
	//    tickSize(tickSize), tickStart(tickStart), yAxis(yAxis), xAxis(xAxis){}

			//			GapMinderWidget(Canvas* c, float x, float y, float w, float h) :
			//GapMinderWidget(c, x, y, w, h, std::string (""), nullptr, nullptr,
			//0, 100, 0, 100, 1.25, 10, 10, x, new LinearScale(), new LinearScale()){} 

#if 0
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
#endif

  void setMinMaxY(float min, float max){
    this->minY = min;
    this->maxY = max;
  }
  void setAxisScale(Scale *yAxis){this->yAxis = yAxis;}
  void chart(const std::vector<float>& yLocations, const std::vector<float>& xLocations, 
  const std::vector<float>& sizes, int rulerIntervalX, int rulerIntervalY, const std::vector <glm::vec4>& c);
  void loadData(std::string sy, std::string sx, std::string ss, int startYear, int endYear);
	//  void animate(int rulerIntervalX, int rulerIntervalY);
  //void chartLog(const float b[], int size, float relativeSpace, const std::string barNames[], int logBase);
  void init() override;
	void update();
};
