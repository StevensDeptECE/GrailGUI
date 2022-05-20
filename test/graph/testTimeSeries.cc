#include "grail/Grail.hh"
#include "TimeSeries.hh"
#include <iostream>

using namespace std;

double sin_tan(double x);

void init(shared_ptr<CompiledWeb> cw) {

	cw->getParent()->inp.init();
  cw->getParent()->inp.assign_mouse("Click for mouse 1\n", 0);
  cw->getParent()->inp.start();



	 uint32_t sizes[] = {12,14,16,30,40};
	 FontFace::loadFonts("TIMES",sizes,5);
	 const Font* font = FontFace::get("TIMES", 16, FontFace::BOLD);

	 Style* cursorStyle = new Style(font, 1,0,0, 0,0,0);
	 Canvas* c = cw->addCanvas(cursorStyle,0,0,GLWin::Width,GLWin::Height);
	 c->addLayer(new Cursor(20,20,cursorStyle,0));


   vector<double> Y_values{10,20,30,40,50,99};
   vector<string> Time_values{"8:02:2003", "8:03:2003", "8:04:2003", "8:05:2003","8:06:2003", "8:07:2003"};
 	TimeSeries s(cw,0,0,500,500);
 	s.addPoints(Y_values, Time_values);
  s.labelTicks();
  s.weekLinePlot();
  s.drawAxes();
  //s.drawTicks();
  s.labelDates();
  s.graphTitle("Y vs Time");
  s.plotPoints();
 }


int main(int argc, char *argv[]) {
	return CompiledWebWindow::initGrail(argc, argv, init);
}
