#pragma once

#include "opengl/Widget2D.hh"

class AxisWidget : public Widget2D {
 protected:
  struct Format {
    int width;
    int precision;
  };
  bool isVert;            // whether the axis will be drawn vertically
  double minBound;        // minimum axis value
  double maxBound;        // maximum axis value
  double tickInterval;    // interval between ticks
  double tickDrawSize;    // length of tick marks in screen coordinates
	//TODO: tickFormat is hardcoded to floating point, what about dates and others?
  Format tickFormat;      // how to format float values on screen
  bool showTicks;         // whether or not to show the ticks
	const Style* axisStyle;
	const Style* axisTextStyle;
  std::string axisTitle;  // axis title
	glm::vec4 axisColor;
  glm::vec4 tickColor;
  double bottomOffset;
	double axisSize;
  void addAxisTitle();

 public:
  AxisWidget(StyledMultiShape2D* m, MultiText* t,
						 const Style* axisStyle, const Style* axisTextStyle,
						 double x, double y, double w, double h,
						 bool isVert,
						 double minBound = 0, double maxBound = 0,
             double tickInterval = 1, double tickDrawSize = 5,
             bool showTicks = true, 
             const std::string& axisTitle = "",
						 int tickFormatWidth = 2,
             int tickFormatPrecision = 2, double bottomOffset = 0);

  virtual ~AxisWidget();

  void setTickDrawSize(double i);
  void setShowTicks(bool b);
  void setIsVert(bool b);
  void setTitle(std::string text);
  void setAxisColor(const glm::vec4& color);
  void setTickColor(const glm::vec4& color);
  void setTickFormat(int width, int precision);
  double getTickInterval();
  double getMinBound();
  double getMaxBound();
  // these three functions are declared as purely virtual, and it will be up to
  // the children to implement them if they make sense, or set them to be
  // private so that the compiler yells at you when using a function that
  // doesn't make sense for that axis type
  virtual void setBounds(double minBound, double maxBound) = 0;
  virtual void setTickInterval(double tickInterval) = 0;
  virtual void setTickLabels(std::vector<std::string> tickLabels) = 0;
	virtual float transform(double v) const = 0;
};

class LinearAxisWidget : public AxisWidget {
  // it doesn't make sense for a linear axis to be able to set its tick labels,
  // logically they should be numbers instead of labels, so the function is set
  // to private so that the compiler complains if a linear axis uses it, and if
  // this somehow gets past the compiler then its an empty definition so no
  // damage can be done
  using AxisWidget::setTickLabels;
  void setTickLabels(std::vector<std::string>) override {}
	double scale, shift; // used for transforming coordinates using this axis
 public:
  LinearAxisWidget(StyledMultiShape2D* m, MultiText* t,
									 const Style* axisStyle, const Style* axisTextStyle,
									 double x, double y, double w, double h, bool isVert);
  void setBounds(double minBound, double maxBound) override;
  void setTickInterval(double tickInterval) override;
  void init() override;
	float transform(double v) const override;
};

class LogAxisWidget : public AxisWidget {
 private:
  int base, power;
	double scale, shift; // used for transforming coordinates
  using AxisWidget::setTickLabels;
  void setTickLabels(std::vector<std::string>) override {}

 public:
  LogAxisWidget(StyledMultiShape2D* m, MultiText* t,
								const Style* axisStyle, const Style* axisTextStyle,
								double x, double y, double w, double h, bool isVert);
  void setBounds(double minBound, double maxBound) override;
  void setTickInterval(double tickInterval) override;
  void init() override;
	float transform(double v) const override;
};

class TextAxisWidget : public AxisWidget {
 private:
  using AxisWidget::setBounds;
  using AxisWidget::setTickInterval;
  void setBounds(double minBound, double maxBound) override{};
  void setTickInterval(double tickInterval) override{};
  std::vector<std::string> tickLabels;

 public:
  TextAxisWidget(StyledMultiShape2D* m, MultiText* t,
								 const Style* axisStyle, const Style* axisTextStyle,
								 double x, double y, double w, double h, bool isVert);
  void setTickLabels(std::vector<std::string> tickLabels) override;
  void init() override;
	float transform(double v) const override;
};
