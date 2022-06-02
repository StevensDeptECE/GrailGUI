#include "BarChart.hh"
#include "BoxChart.hh"
#include "CandlestickChart.hh"
#include "LineGraph.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/NavigationBar.hh"

using namespace std;
using namespace grail;

class ChartNavBar : public NavigationBar {
 public:
  ChartNavBar(GLWin* w);
};

ChartNavBar::ChartNavBar(GLWin* w) : NavigationBar(w, 0, 0, 400, 50, 10) {
  setButtonStyle((Font*)FontFace::get("TIMES", 28, FontFace::BOLD),
                 grail::black, grail::yellow, 2);

  ButtonWidget* bar = addButton("Bar", "tab 1");
  bar->setAction([w]() { w->switchTab(3); });

  ButtonWidget* box = addButton("Box", "tab 2");
  box->setAction([w]() { w->switchTab(2); });

  ButtonWidget* line = addButton("Line", "tab 3");
  line->setAction([w]() { w->switchTab(1); });

  ButtonWidget* cstick = addButton("CStick", "tab 4");
  cstick->setAction([w]() { w->switchTab(0); });

  fitBarDimensions(5, 5);
  drawBarBox(grail::black, grail::gray, 2);
}

void grailmain(int argc, char* argv[], GLWin* w) {
  new ChartNavBar(w);
  new CandlestickChart(w);
  new LineGraph(w);
  new BoxChart(w);
  new BarChart(w);
}