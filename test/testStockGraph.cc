#include "grail/Grail.hh"
#include "opengl/primitivesV2/MultiText.hh"
#include "grail/LineGraph.hh"
#include "util/DynArray.hh"
#include <sstream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
using namespace std;

void parseData(const char textFile[], vector<double>& x, vector<double>& y) {
	ifstream in(textFile);
    string line;
    // open file
    // read line
    // parse date and open from line
    // convert date to numerical form
    // if first date set it to the date and normalize to 0
    // push to vector x and y
    // repeat until EOF
    bool firstDate = true;
    uint32_t year = 0, month = 0, day = 0;
    double open = 0;
    uint64_t date, baseDate;
    getline(in, line);
	while (getline(in, line)) {
        //istringstream sstream;
        //sstream.str(line);
        //sstream >> year;
        //sstream >> month >> day >> open;
        
        sscanf(line.c_str(), "%u-%u-%u,%lf", &year, &month, &day, &open);
        date = year * 365 + month * 32 + day;
        if (firstDate) {
            baseDate = date;
            firstDate = false;
        }
        cout << "base, date: " << baseDate << ", " << date-baseDate << endl;
        cout << "read: " << year << ", " << month << ", " << day << ", " << open << endl;
        cout << "data point: (" << (date-baseDate) << ", " << open << ")" << endl;
        x.push_back(double(date-baseDate));
        y.push_back(open);
    }
}

class TestStockGraph : public Grail {
public:
TestStockGraph(uint32_t w, uint32_t h) : Grail(w,h) {}
	void init() {
		const uint32_t w = GLWin::Width, h = GLWin::Height;
		Style* s = getDefaultStyle();
		const Font* font = getDefaultFont();
		//		Canvas*c = currentTab()->getMainCanvas();

		Canvas* c = currentTab()->addCanvas(s, 0, 300, w, h-300);
		StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(s));
		MultiText* t = c->addLayer(new MultiText(s, 20));
		t->init();
        // Begin setting up LineGraph
        string title("Dow Jones Stock Price: 1985-Present");

		vector<double> testx;
		vector<vector<double>> testy;

		int numGraphs = 1;
		for (int i = 0; i < numGraphs; i++)
			testy.push_back(vector<double>());

        parseData("res/Dow_daily.csv", testx, testy[0]);

		vector<char> shapes(numGraphs);
		shapes[0] = 'c';
        
	
		LineGraph *g = new LineGraph(title, t, m, s, 50, 50, 900, 600, shapes, testx, testy);
    }
};

int main(int argc, char *argv[]) {
  return Grail::init(new TestStockGraph(1024,1024), "out.cml");
}