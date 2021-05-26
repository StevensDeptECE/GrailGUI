#include "opengl/GrailGUI.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GraphWidget.hh"
#include "opengl/BarChartWidget.hh"
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

    GraphWidget graph(gui, guiText, 0, 300, 400, 200);
    graph.setTitleStyle(s);
    graph.setLineStyle(s);
    //graph.setXAxisStyle(s);
    //graph.setYAxisStyle(s);
    //graph.setTitle("sin(x)");
    //graph.setXAxis("x");
    //graph.setYAxis("x");
    float x[] = {100, 200, 300};
    float y[] = {50, 350, 222};
    //graph.lineGraph(x, y);


    BarChartWidget chart(gui, guiText, 0, 300, 400, 200);

  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestWidgets(), 512, 512);
}
