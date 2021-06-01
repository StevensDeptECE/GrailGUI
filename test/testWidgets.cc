#include "opengl/GrailGUI.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GraphWidget.hh"
#include "opengl/BarChartWidget.hh"
#include <string>
#include <vector>
using namespace std;

class TestWidgets : public GLWin {
 public:
  void init() {
    const Style* s = new Style("TIMES", 24, 1,
          0,1,0,0,
          0,0,0,0);
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    MultiText* guiText = c->getGuiText();
    ButtonWidget b(gui, guiText, "hello", 0, 0, 100, 50);
    b.init();

    GraphWidget graph(gui, guiText, 0, 300, 400, 200);
    graph.setTitleStyle(s);
    graph.setLineStyle(s);
    //graph.setXAxisStyle(s);
    //graph.setYAxisStyle(s);
    //graph.setTitle("sin(x)");
    //graph.setXAxis("x");
    //graph.setYAxis("x");
    float x[] = {100, 10, 1000, 150, 10000};
    vector<float> y = {150, 350, 222, 100, 300};
    //graph.lineGraph(x, y);

    vector<string> labels = {"bar","big bar","other bar","small bar", "barrr"};
    BarChartWidget chart(gui, guiText, 50, 50, 400, 200);
    chart.chart(y, 0.25, labels, 50);
    chart.title("Title");
    chart.init();

    #if 0
    std::string labels2[] = {"bar 1","bar 2","bar 3","bar 4", "bar 5"};
    BarChartWidget chart2(gui, guiText, 50, 320, 400, 200);
    chart2.chartLog(x, 5, 0.25, labels2, 10);
    chart2.title("Title Log graph");
    chart2.init();
    #endif
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestWidgets(), 600, 600);
}
