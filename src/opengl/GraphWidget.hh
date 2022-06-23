#pragma once

#include "opengl/AxisWidget.hh"
#include "opengl/GraphStyle.hh"
#include "opengl/SuperWidget2D.hh"

class GraphWidget : public SuperWidget2D {
 public:
  enum AxisType {
    // Functional goes from 1-255 as powers of 2
    // Bits 1, 2, 4, etc
    LINEAR = 1,
    LOGARITHMIC = 2,
    // Discrete goes from 255 onwards
    // 256, 512, 1024
    TEXT = 256,
  };

 protected:
  constexpr static uint32_t FUNCTIONS_PERMITTED = 0xFF;
  constexpr static uint32_t DISCRETE_PERMITTED = 0xFF00;

  std::string graphTitle;
  const GraphStyle* s;  // all look and feel for graphs encapsulated here
  uint32_t allowedXAxis;
  uint32_t allowedYAxis;
  AxisType xAxisType;
  AxisType yAxisType;

  void commonRender();

  AxisWidget* createAxis(uint32_t allowed, AxisType typ, const Style* axisStyle,
                         const Style* axisTextStyle, bool isVert,
                         AxisType& out);

 public:
  AxisWidget* xAxis;
  AxisWidget* yAxis;

  GraphWidget(Canvas* c, double x, double y, double w, double h,
              uint32_t allowedXAxis, uint32_t allowedYAxis, AxisType xAxisType,
              AxisType yAxisType, const GraphStyle* s);
  ~GraphWidget();
  GraphWidget(const GraphWidget&) = delete;
  GraphWidget& operator=(const GraphWidget&) = delete;

  /**
   * @brief Summary
   * @details Description
   * @param[in] a Description
   */
  void createXAxis(AxisType a);

  /**
   * @brief Summary
   * @details Description
   * @param[in] a Description
   */
  void createYAxis(AxisType a);

  /**
   * @brief Summary
   * @details Description
   * @param[in] text Description
   */
  void setGraphTitle(const std::string& text);

  virtual void setStyle(const GraphStyle* s);
  virtual void init() = 0;
};
