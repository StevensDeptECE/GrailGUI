#include "BarChart.hh"
#include "BoxChart.hh"
#include "CandlestickChart.hh"
#include "LineGraph.hh"
#include "opengl/AutoNavBar.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

void grailmain(int argc, char* argv[], GLWin* w) {
  AutoNavBar* bar = new AutoNavBar(w, 0, 0, 10, 5, 5, false, true);
  bar->setButtonStyle(FontFace::get("TIMES", 28, FontFace::BOLD), grail::black,
                      grail::yellow, 2);
  new CandlestickChart(w);
  new LineGraph(w);
  new BoxChart(w);
  new BarChart(w);
}