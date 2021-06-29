#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/LineGraphWidget.hh"
using namespace std;
using namespace grail;

class StockDemo : public GLWin {
 public:
  StockDemo() : GLWin(0x000000, 0xCCCCCC, "Stock Demo") {}

  vector<float> openFile(const char name[]) {
    fstream file;
    vector<float> v;
    // Read file
    file.open(name, ios::in);  // Open file
    if (file.is_open()) {      // If file has correctly opened...
      // Output debug message
      cout << "File correctly opened" << endl;

      string temp;
      float x, y;
      // Dynamically store data into array
      while (file.peek() != EOF) {  // ... and while there are no errors,
        getline(file, temp, '\n');
        v.push_back(atof(temp.c_str()));
      }
    } else
      cout << "Unable to open file" << endl;
    file.close();
    return v;
  }

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    MultiText* guiText = c->getGuiText();

    vector<float> GME = openFile("GME.csv");
    vector<float> AAPL = openFile("AAPL.csv");
    vector<float> NTDOY = openFile("NTDOY.csv");
    vector<float> AMC = openFile("AMC.csv");

    vector<float> day;
    for (int i = 0; i < AMC.size(); i++) day.push_back(i);

#if 0
    LineGraphWidget chart(gui, guiText, 100, 320, 800, 400);
    chart.axes("Days",4,"Price ($)",9,day.size(),75,day.size()-100,0,10,10,day.size()-100,day.size());
    chart.add(day, GME, blue, "GME");
    //chart.add(day, AAPL, red, "AAPL");
    chart.add(day, NTDOY, green, "NTDOY");
    chart.add(day, AMC, purple, "AMC");
    chart.title("Daily High of GME, and NTDOY Stocks Since 2010");
    chart.legend(100,800);
    chart.init(); 
    //TODO:Add xAxis and yAxis label funcs
    //TODO:Add legend to all graphwidgets
    //TODO:Fix chart axis ticker in chart(), maybe move to init()
    //TODO:Iterations of chart funcs
    //TODO:Custom Locations of axes labels
    //TODO:Comment code
#endif
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new StockDemo()); }
