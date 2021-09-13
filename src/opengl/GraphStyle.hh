#pragma once

#include "opengl/Animated.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class GraphStyle {
 public:

	inline glm::vec4 u32_to_vec4(uint32_t c) {
		return glm::vec4((c >> 24) / 255.0f,
										 ((c >> 16) & 0xFF) / 255.0f,
										 ((c >> 8) & 0xFF) / 255.0f,
										 1.0f);
	}
	Font* titleFont;
	Font* axisFont;
	glm::vec4 borderColor;
	glm::vec4 axisColor;
	glm::vec4 lineColor;
	glm::vec4 xTickColor, yTickColor;
	glm::vec4 textColor;
	glm::vec4 bgColor;
	glm::vec4 pointColor;

	bool drawBorder;
	bool drawAxes;
	bool drawTicks;
  bool drawLines;
	bool drawMarkers;
	
  Style borderStyle;
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
	
	float xTickSize; // length of ticks on the x axis (screen coords)
	float yTickSize; // length of ticks on the y axis (screen coords)
	float pointSize;
	float boxWidth; // width of boxes for candlestick and bar charts
	float spaceBetweenBars; // space between bars for bar or candlestick graphs
 public:
	void setDefaults();
	void setDefaultColorLists();
	void sparkline();
	/*
		Define a simplified style with common color scheme.
		If you want a garish scheme, you will need to set individual colors for each different element,
		but for best look and feel, most of these colors should be the same.
		For example, usually we do not have different colors for the rectangle around a graph, the tick marks,
		and the text for x and y labels, but if you really want to, you can.
	 */
  GraphStyle(const char faceName[],  // the font family used for all text in the graph
						 float titleTextSize, //
						 float axisTextSize,
						 uint32_t borderColor,
						 uint32_t axisColor,
						 uint32_t lineColor,
						 uint32_t textColor,
						 uint32_t bgColor,
						 uint32_t pointColor,
						 uint32_t borderThickness = 2,
						 uint32_t lineThickness = 1,
						 uint32_t pointSize = 4) :
		borderColor(u32_to_vec4(borderColor)),
		axisColor(u32_to_vec4(axisColor)),
		lineColor(u32_to_vec4(lineColor)),
		textColor(u32_to_vec4(textColor)),
		bgColor(u32_to_vec4(bgColor)),
		pointColor(u32_to_vec4(pointColor)),
		// two lines and the overall title
		borderStyle(faceName, titleTextSize, 1, this->bgColor, this->borderColor, borderThickness),
		titleStyle(faceName, titleTextSize, 1, this->bgColor, this->textColor, 1),
		// Color and thickness of x axis lines
		xAxisStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness),
		// font, size, color of x axis text
		xAxisTextStyle(faceName, axisTextSize, 1, this->bgColor, this->textColor, 1),
		// Color and thickness of y axis lines
		yAxisStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness),
		// font, size, color of x axis text
		yAxisTextStyle(faceName, axisTextSize, 1, this->bgColor, this->textColor, 1),
		// Color and thickness of whisker lines (box and candlestick graphs)
		whiskerStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness),
		// Color and thickness of whisker lines (box and candlestick graphs)
		lineStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness),
		// controls the thickness of box outlines
		boxStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness),
		barStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness),
		dataStyle(faceName, axisTextSize, 1, this->bgColor, this->lineColor, lineThickness) {
		
		setDefaults();
		setDefaultColorLists();
	}

	void setBorderColor(uint32_t c) { borderColor = u32_to_vec4(c); }
	void setTickColor(uint32_t c) { xTickColor = yTickColor = u32_to_vec4(c); }
	void setLineColor(uint32_t c) { lineColor = u32_to_vec4(c); }
	void setTextColor(uint32_t c) { textColor = u32_to_vec4(c); }
	void setBackgroundColor(uint32_t c) { bgColor = u32_to_vec4(c); }
	void setPointColor(uint32_t c) { pointColor = u32_to_vec4(c); }

	void setBoxColors(const std::vector<glm::vec4>& boxColors) {
		this->boxColors = boxColors;
	}

	void setWhiskerColors(const std::vector<glm::vec4>& whiskerColors) {
		this->whiskerColors = whiskerColors;
	}

	void setOutlineColors(const std::vector<glm::vec4>& outlineColors) {
		this->outlineColors = outlineColors;
	}

  /**
   * @brief set the common style for graph border color
   * @details Used by all graphs, right now this is the box around the graph
	 * Perhaps later this can be augmented with more complex borders
   * @param[in] s Description
   */
  void setBorderStyle(const Style* s) {
		borderStyle = *s;
	}


  /**
   * @brief set a style for the x axis label
   * @details Separate style for the line, the text and the tick marks
   * @param[in] xAxisTextStyle Description
   */
  void setXAxisTextStyle(const Style* xAxisTextStyle) {
		this->xAxisTextStyle = *xAxisTextStyle;
	}

  /**
   * @brief set a style 
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
	void setPointSize(uint32_t size) {
		pointSize = size;
	}
	void setTickSize(float tickSize) {
		xTickSize = yTickSize = tickSize;
	}
	void setBoxWidth(float w) {
		boxWidth = w;
	}
	//	GraphStyle(const char faceName[], uint32_t titleSize, uint32_t axisSize)
	
	static GraphStyle* steelblue;
	static GraphStyle* forest;
	static GraphStyle* shadow;
	static GraphStyle* colorful;
	static void classInit();
	static void classCleanup();
};
