#pragma once
#include <memory>
#include <vector>

#include "opengl/Canvas.hh"
#include "opengl/primitivesV2/MultiShape2D.hh"
#include "opengl/primitivesV2/StyledMultiShape2D.hh"

class CompiledWeb;
class TimeSeries {
 private:
  struct point {
    double value;
    double time;
  };
  struct candlestick {
    double day;
    double open;
    double close;
    double high;
    double low;
  };
  Canvas* c;
  Style* s;
  Style* textStyle;
  Style* titleStyle;
  Style* tickStyle;
  MultiShape2D* m;
  StyledMultiShape2D* sm;
  std::vector<point> Points;
  std::vector<candlestick> CandleSticks;
  // std::vector<double> Julian_Numbers;
  uint32_t cWidth, cHeight;
  double maxClose();
  double maxYValue();
  double minYValue();
  double minJulianNumber();
  double maxJulianNumber();
  std::vector<double> setJulianDates(std::vector<std::string> time_values);
  uint32_t num_ticks;

 public:
  TimeSeries(std::shared_ptr<CompiledWeb> cw, float x, float y, float w,
             float h);
  void addPoints(std::vector<double>& y_values,
                 std::vector<std::string>& time_values);
  void drawAxes();
  void weekLinePlot();
  // void drawTicks(uint32_t number_ticks = 5);
  void labelDates();
  void labelTicks(uint32_t number_ticks = 5);
  void graphTitle(std::string title);
  void plotPoints();
  void addCandleSticks(std::vector<std::string> Times,
                       std::vector<double> Opens, std::vector<double> Closes);
  void drawCandleStick(candlestick c);
  void plotCandleStick();
};
