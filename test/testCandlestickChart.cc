#include "opengl/CandlestickChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestCandlestickChart : public Animated {
 public:
  TestCandlestickChart(Tab* tab, const GraphStyle* gs) : Animated(tab) {
    MainCanvas *c = tab->getMainCanvas();

    vector<double> data = {
        153.25, 154.16, 152.99, 153.68, 153.34, 153.73, 152.15, 152.73, 152.66,
        153.58, 151.46, 153.57, 153.71, 153.9,  152.17, 153.26, 153.46, 154.53,
        153.36, 153.92, 153.08, 153.31, 151.91, 152.93, 153.63, 154.88, 153.5,
        154.83, 154.41, 154.93, 154.01, 154.74, 155.25, 156.17, 154.93, 155.28,
        155.5,  157.43, 154.54, 155.34, 155.41, 156.42, 155.19, 155.99, 155.79,
        156.81, 155.17, 155.44, 156.17, 156.72, 155.85, 156.43, 157.33, 159.43,
        157.09, 159.31, 159.21, 160.29, 158.66, 159.9,  159.85, 160.13, 159.03,
        159.19, 156.19, 156.52, 154.46, 155.42, 156.05, 157.18, 155.4,  155.69,
        156.33, 157.13, 154.94, 155.61, 155.73, 156.86, 155.64, 156.54, 156.35,
        156.99, 154.71, 155.85, 156.67, 157.26, 156.22, 156.85, 158.72, 163.01,
        158.13, 162.47, 163.3,  167.47, 163.13, 166.12, 167.3,  169.04, 166.34,
        168.43, 169.26, 169.33, 165.02, 166.29, 167.04, 167.9,  164.69, 167.51,
        173.38, 173.64, 170.51, 171.88, 171.75, 174.36, 171.1,  173.63, 173.29,
        174.51, 173.29, 174.18, 174.03, 175.61, 173.71, 175.61, 174.48, 175.46,
        172.52, 175.25, 175.11, 175.38, 174.27, 174.67};
    CandlestickChartWidget ccw(c, 100, 100, 850, 400,
															 GraphWidget::AxisType::LINEAR, GraphWidget::AxisType::LINEAR, gs);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    ccw.setTitle("Test Title");

    // bar chart widget specific bits
    // ccw.setBoxWidth(5);
    // ccw.setBoxColors(boxColors);
    // ccw.setBoxOutlineColors(outlineColors);

    // set relevant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    ccw.xAxis->setTitle("Date");
		//    ccw.xAxis->setTickDrawSize(7);
    //ccw.xAxis->setAxisColor(grail::green);
    //ccw.xAxis->setTickColor(grail::purple);

    // y axis stuff
    ccw.setData(data);

    // set y axis parameters
    //ccw.yAxis->setIsVert(true);

    // linear
    ccw.yAxis->setBounds(140, 180);
    ccw.yAxis->setTickInterval(2);

    //ccw.yAxis->setTickDrawSize(10);
    //ccw.yAxis->setShowTicks(true);
    ccw.yAxis->setTitle("Price");
    //ccw.yAxis->setAxisColor(grail::yellow);
    //ccw.yAxis->setTickColor(grail::red);

    ccw.init();
  }
};

void grailmain(int argc, char *argv[], GLWin* w, Tab* tab) {
  new TestCandlestickChart(tab, GraphStyle::steelblue);
}
