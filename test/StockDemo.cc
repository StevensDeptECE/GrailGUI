#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/LineGraphWidget.hh"
using namespace std;
using namespace grail;

class StockDemo : public GLWin {
 public:
  StockDemo() : GLWin(0x000000, 0xCCCCCC, "Stock Demo") {}

  vector<float> openFile(char name[]){
    fstream file;
		vector <float> v; 
		// Read file
		file.open(name, ios::in); // Open file
		if (file.is_open()) { // If file has correctly opened...
			// Output debug message
			cout << "File correctly opened" << endl;

			string temp;
			float x,y;
			// Dynamically store data into array
			while (file.peek()!=EOF) { // ... and while there are no errors,
				getline(file,temp,'\n');
				v.push_back(atof(temp.c_str()));
			}
		}
		else cout << "Unable to open file" << endl;
		file.close();
    return v;
  }

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    MultiText* guiText = c->getGuiText();

		vector <float> GME = openFile("GME.csv"); 
    vector <float> AAPL = openFile("AAPL.csv"); 
		
    vector<float> day; 
    for(int i = 0; i < GME.size(); i++)
      day.push_back(i);

    LineGraphWidget chart(gui, guiText, 50, 320, 800, 400);
    chart.chart(day, GME, 50, 50, new LinearScale(), new LinearScale());
    chart.title("Daily High of GME and AAPL Stock Since 2010");
    chart.add(day,AAPL,50,50,new LinearScale(),new LinearScale(),red);
    chart.init();

  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new StockDemo());
}
