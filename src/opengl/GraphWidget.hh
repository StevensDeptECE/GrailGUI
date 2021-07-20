#pragma once

#include "opengl/AxisWidget.hh"
#include "opengl/SuperWidget2D.hh"

class Style;

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
  const Style* baseStyle;
  const Style* xAxisStyle;
  const Style* xAxisTextStyle;
  const Style* yAxisStyle;
  const Style* yAxisTextStyle;
  uint32_t allowedXAxis;
  uint32_t allowedYAxis;
  AxisType xAxisType;
  AxisType yAxisType;

  void commonRender();

  AxisWidget* createAxis(uint32_t allowed, AxisType typ, const Style* axisStyle,
                         const Style* axisTextStyle, bool isVert,
                         AxisType* out);

 public:
  AxisWidget* xAxis;
  AxisWidget* yAxis;

  GraphWidget(Canvas* c, double x, double y, double w, double h,
              uint32_t allowedXAxis, uint32_t allowedYAxis,
              Style* baseStyle = nullptr, Style* xAxisStyle = nullptr,
              Style* xAxisTextStyle = nullptr, Style* yAxisStyle = nullptr,
              Style* yAxisTextStyle = nullptr);
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
   * @param[in] s Description
   */
  void setBaseStyle(const Style* s);

  /**
   * @brief Summary
   * @details Description
   * @param[in] text Description
   */
  void setGraphTitle(std::string text);

  /**
   * @brief Summary
   * @details Description
   * @param[in] xAxisTextStyle Description
   */
  void setXAxisTextStyle(const Style* xAxisTextStyle);

  /**
   * @brief Summary
   * @details Description
   * @param[in] yAxisTextStyle Description
   */
  void setYAxisTextStyle(const Style* yAxisTextStyle);

  /**
   * @brief Summary
   * @details Description
   * @param[in] xAxisStyle Description
   */
  void setXAxisStyle(const Style* xAxisStyle);

  /**
   * @brief Summary
   * @details Description
   * @param[in] yAxisStyle Description
   */
  void setYAxisStyle(const Style* yAxisStyle);
  virtual void init() = 0;
};
