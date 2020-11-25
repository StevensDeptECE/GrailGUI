#include "grail/Grail.hh"
#include "TimeSeries.hh"
#include <iostream>

using namespace std;

class CandleStick : public Canvas {
	void init() {
		const Font* font = g->getDefaultFont();
		Style* cursorStyle = new Style(font, 1,0,0, 0,0,0);
		//c->addLayer(new Cursor(20,20,cursorStyle,0));

		vector<double> opens{10,20,30,40,50,99};
		vector<double> closes{9,21,31,39,51,98};
		vector<string> timeValues
			{
			 "8:02:2003", "8:03:2003", "8:04:2003",
			 "8:05:2003","8:06:2003", "8:07:2003"
			};
		TimeSeries s(g->cw,0,0,500,500);
		s.addCandleSticks(Time_values, Opens, Closes);
		s.drawAxes();
		//s.drawTicks();
		//s.labelDates();
		s.graphTitle("CandleStick");
		//s.plotPoints();
		s.plotCandleStick();
	}
};

int main(int argc, char *argv[]) {
  return GLWin::init(new CandleStick(), 1024, 1024);
}
