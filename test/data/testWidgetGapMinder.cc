#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include "opengl/GapMinderWidget.hh"
#include "data/GapMinderLoader.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

int argc2;
char *argv2;

class TestWidgets : public GLWin {
 public:

  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}

  void testGapMinder(StyledMultiShape2D *gui, MultiText *guiText) {

    std::string grail = getenv("GRAIL");
    grail += "/test/res/GapMinder/";

    grail += argc2 > 1 ? (argv2) : "GapMinderDBFile";
    GapMinderLoader gml(grail.c_str());

    const GapMinderLoader::Dataset* d = gml.getDataset("gdp_per_capita.csv");

    const GapMinderLoader::Dataset* d2 = gml.getDataset("ddf--datapoints--poisonings_deaths_per_100000_people.csv");

    const GapMinderLoader::Dataset* d3 = gml.getDataset("ddf--datapoints--vacc_rate--by--country--time.csv");

    vector<float> x1 = gml.getAllDataOneYear(2000, d);

    cout << "----------------------------------" << endl;
    cout << x1[0] << endl;
    cout << x1[10] << endl;
    cout << x1[100] << endl;

    vector<float> x2;

    vector<float> y1 = gml.getAllDataOneYear(2000, d2);

    cout << "----------------------------------" << endl;
    cout << y1[0] << endl;
    cout << y1[10] << endl;
    cout << y1[100] << endl;

    vector<float> y2;

    vector<float> s1 = gml.getAllDataOneYear(2000, d3);

    cout << "----------------------------------" << endl;
    cout << s1[0] << endl;
    cout << s1[10] << endl;
    cout << s1[100] << endl;

    vector<float> s2;

    cout << "vectors" << endl;

    vector <glm::vec4> colorContinent = {
      grail::yellow, grail::blue, grail::cyan, grail::green, grail::pink, grail::red
    };

    vector<glm::vec4> c2;

    cout << "thing" << endl;

    for (int i = 0; i < x1.size(); i++){
      if(x1[i] < 1000000 && y1[i] < 1000000 && s1[i] < 1000000){
        x2.push_back(x1[i]);
        y2.push_back(y1[i]);
        s2.push_back(s1[i]);
        c2.push_back(colorContinent[gml.continents[i]]);
      }
    }

    float maxX2 = x2[0];
    //for (int i = 0; i < x2.size(); i++){
    //  if (x2[i] > maxX2){
    //    maxX2 = x2[i];
    //  }
    //}
    //cout << "max: " << maxX2 << endl;


    GapMinderWidget chart(gui, guiText, 75, 50, 900, 450, x2, y2);
    chart.chart(y2, x2, s2, 10000, 2, c2);

    chart.setTitle("Title");
    chart.init();
  }

  void init() {

    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    const Style *s2 = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c->addLayer(new MultiText(c, s));
    
    testGapMinder(gui, guiText);
    
  }
};

int main(int argc, char *argv[]) {

  argc2 = argc;
  argv2 = argv[1];

  
  return GLWin::init(new TestWidgets(), 1024, 600);

}
