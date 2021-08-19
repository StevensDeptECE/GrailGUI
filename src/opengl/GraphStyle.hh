#pragma once

#include "opengl/Animated.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class GraphStyle : public Animated {
 public:
  Style baseStyle;
	Style titleStyle;
  Style xAxisStyle;
  Style xAxisTextStyle;
  Style yAxisStyle;
  Style yAxisTextStyle;
  Style whiskerStyle;
	Style lineStyle;
	Style barStyle;
  Style boxStyle;
  Style dataStyle;
	vector<glm::vec4> boxColors;
	vector<glm::vec4> whiskerColors;
	vector<glm::vec4> outlineColors;
	vector<glm::vec4> barColors;			//TODO: eliminate redundant colors, use style instead?
	vector<glm::vec4> barOutlineColors;
	glm::vec4 lineColor;
	glm::vec4 pointColor;
	
	float boxWidth; // width of boxes for candlestick
	float xTickDrawSize;
	float yTickDrawSize;
	float pointSize;
 public:
  GraphStyle(Tab* tab, const char faceName[], uint32_t titleSize, uint32_t axisSize)
		: Animated(tab),
			// two lines and the overall title
			baseStyle(faceName, axisSize, 1, 0, 0, 0, 0.3, 0.3, 0.3, 5),
			titleStyle(faceName, titleSize, 1, 0, 0, 0, 0, 0, 0, 5),
			// will control how thick lines for x axis are drawn
			xAxisStyle(faceName, axisSize, 1, 0, 0, 0, 0, 1, 0, 4),
			// controls the font, size, and color of x axis text
			xAxisTextStyle(faceName, axisSize, 1, 0, 0, 0, 1, 0, 0, 3),
			// will control how thick lines for y axis are drawn
			yAxisStyle(faceName, axisSize, 1, 0, 0, 0, 0, 0, 1, 2),
			// controls the font, size, and color of y axis text
			yAxisTextStyle(faceName, axisSize, 1, 0, 0, 0, 0.5, 0.5, 0.5, 1),
			// controls the thickness of whisker lines drawn
			whiskerStyle(faceName, axisSize, 1, 0, 0, 0, 0.5, 0.5, 0.5, 3),
			lineStyle(faceName, axisSize, 1, 0, 0, 0, 1, 0, 1, 2),
			// controls the thickness of box outlines
			boxStyle(faceName, 12, 1, 0, 0, 0, 1, 0, 1, 3),
			barStyle(faceName, 12, 1, 0, 0, 0, 1, 0, 1, 3),
			dataStyle(faceName, 12, 1, 0, 0, 0, 1, 0, 1, 3),
			xTickDrawSize(7),
			yTickDrawSize(10),
			pointSize(4)
	{
		boxColors = {grail::red, grail::green, grail::blue};
		whiskerColors = {grail::cyan, grail::purple};
		outlineColors = {grail::darkblue, grail::darkgreen};
		pointColor = grail::blue;
		lineColor = grail::black;
		barColors = {grail::blue, grail::green, grail::red,
			grail::darkblue, grail::darkgreen, grail::darkred };
		barOutlineColors = {grail::gray, grail::black, grail::yellow};
	}

  /**
   * @brief Summary
   * @details Description
   * @param[in] s Description
   */
  void setBaseStyle(const Style* s) {}


  /**
   * @brief Summary
   * @details Description
   * @param[in] xAxisTextStyle Description
   */
  void setXAxisTextStyle(const Style* xAxisTextStyle) {}

  /**
   * @brief Summary
   * @details Description
   * @param[in] yAxisTextStyle Description
   */
  void setYAxisTextStyle(const Style* yAxisTextStyle) {}

  /**
   * @brief Summary
   * @details Description
   * @param[in] xAxisStyle Description
   */
  void setXAxisStyle(const Style* xAxisStyle) {}

  /**
   * @brief Summary
   * @details Description
   * @param[in] yAxisStyle Description
   */
  void setYAxisStyle(const Style* yAxisStyle) {}

	//	GraphStyle(const char faceName[], uint32_t titleSize, uint32_t axisSize)
	
	static GraphStyle steelblue;
	static GraphStyle forest;
	static GraphStyle shadow;
};
