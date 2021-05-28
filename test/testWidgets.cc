#include "opengl/GrailGUI.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GraphWidget.hh"
#include "opengl/BarChartWidget.hh"
#include <string>
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
    float x[] = {100, 200, 300, 100, 100};
    float y[] = {150, 350, 222, 100, 300};
    //graph.lineGraph(x, y);

    std::string lables[] = {"bar","big bar","other bar","small bar", "barrr"};
    BarChartWidget chart(gui, guiText, 50, 50, 400, 200);
    chart.chart(y, 5, 0.25, lables, grail::green);
    chart.init();

  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestWidgets(), 512, 512);
}
