#pragma once
#include <vector>
#include "opengl/Canvas.hh"
#include <memory>

class StyledMultiShape2D;
class MultiText;
class Style
class BarChart{
private:
	Canvas* c;
	Style* s;
	StyledMultiShape2D* sm;
  MultiText* mt;
	Style* textStyle;
	std::vector<double> Y_values;
	std::vector<std::string> Y_values_text;
	std::vector<std::string> Axis_labels;
	uint32_t cWidth, cHeight;
	double rectWidth;
public:
  BarChart(std::shared_ptr<CompiledWeb> cw, float x, float y, float w, float h);
  void add_Point(double add_y, std::string add_label);
  void add_Points(std::vector<double> &y, std::vector<std::string> &axis_labels);
	void drawAxes();
  double maxValue(std::vector<double> &Y_values);
  double minValue(std::vector<double> &Y_values);
	void drawTicks(std::vector<double> &Y_values);
  void tickLabel(std::vector<double> &Y_values);
	void setRectangleWidth();
	double getRectangleWidth();
	void drawSingleBar(double y_value);
	void drawColoredBar(double y_value);
	void drawBars(std::vector<double> &Y_values);
	void drawBarLabel(std::vector<std::string> &Axis_labels);
	void graphTitle(std::string title);
};
