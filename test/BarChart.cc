
#include "BarChart.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include "opengl/GLWinFonts.hh"
#include "opengl/GLWin.hh"
#include "opengl/Style.hh"
#include "opengl/GLMath.hh"
#include "plot.hh"
#include <algorithm>
#include <numeric>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <string>
#include <climits>
#include "grail/CompiledWebWindow.hh"
#include <string>
using namespace std;

BarChart::BarChart(shared_ptr<CompiledWeb> cw, float x, float y, float w, float h){
	const Font* font = FontFace::get("TIMES", 16, FontFace::BOLD);
	s = new Style(font,1,0,0,.2,.4,.5);
	s->setLineWidth(2);
	s->setShaderIndex(CompiledWebWindow::COMMON_SHADER);
	textStyle = new Style(font, 0, 0, 0, 0, 0, 0);
	textStyle->setShaderIndex(CompiledWebWindow::TEXT_SHADER);
	c = cw->addCanvas(s,x,y,w,h);
	m = c->addLayer(new MultiShape2D(s));
	sm = c->addLayer(new StyledMultiShape2D(s));
	cWidth = c->getWidth();
	cHeight = c->getHeight();
}

void BarChart::add_Point(double add_y, string add_label){
	Y_values.push_back(add_y);
	Axis_labels.push_back(add_label);
}

void BarChart::add_Points(vector<double> &y, vector<string> &axis_labels){
	Y_values.insert(Y_values.end(), y.begin(), y.end());
	Axis_labels.insert(Axis_labels.end(), axis_labels.begin(), axis_labels.end());
  cout << "Y_values size: " << Y_values.size() << endl;
}

void BarChart::drawAxes(){
	m->drawLine(40, 40, 40, cHeight-40);
	m->drawLine(40, cHeight-40, cWidth, cHeight -40);
}
double BarChart::maxValue(vector<double> &Y_values){
	double maxVal =  std::numeric_limits<double>::min();
	for(int i = 0; i < Y_values.size(); i++){
		if(Y_values[i] >= maxVal){
			maxVal = Y_values[i];
		}
	}
	return maxVal;
}
double BarChart::minValue(vector<double> &Y_values){
  double minValue = std::numeric_limits<double>::max();
  for(int i = 0; i < Y_values.size(); i++){
    if(Y_values[i] < minValue){
      minValue = Y_values[i];}
  }
  return minValue;
}
//This method must be fixed to work all the time
void BarChart::drawTicks(vector<double> &Y_values){
	double y_pos = cHeight-40-(cHeight-80)/5;
	for(int i = 0; i <= 5; i++){
		m->drawLine(20,y_pos,40, y_pos);
		y_pos -= (cHeight-80)/5;
	}
}
void BarChart::tickLabel(vector<double> &Y_values){
  uint32_t diff = (maxValue(Y_values)-minValue(Y_values))/3;
  uint32_t y_max = maxValue(Y_values);
  uint32_t y_min = minValue(Y_values);
  cout << "YMAX: " << y_max << endl;
  cout << "YMIN: " << y_min << endl;
  string text_y;
  vector<string> Y_values_text{};
  
   stringstream ss;
  // ss << fixed << setprecision(1) << y_max;
  // text_y = ss.str();
  text_y = to_string(y_max);
  Y_values_text.push_back(text_y);
  uint32_t y = y_max;
  for(int i = 0; i <= 2; i ++) {
    y -= diff;
    text_y = to_string(y);
    Y_values_text.push_back(text_y);
  }
 
  text_y = to_string(y_min);
  Y_values_text.push_back(text_y);
  
	const Font* font = FontFace::get("TIMES", 16, FontFace::BOLD);
	Style* tickStyle = new Style(font, 0, 0, 0, 0, 0, 0);
	double tick_pos = 55;
	for (int i = 0; i < Y_values_text.size(); i++){
			c->addLayer(new Text(20,tick_pos,tickStyle,Y_values_text[i]));
			tick_pos += (cHeight-80)/5;
	}
}

void BarChart::setRectangleWidth(){
  cout << "Y values size : " << Y_values.size() << endl;
	rectWidth = (cWidth-20-.1*cWidth)/Y_values.size();
  cout << "Rect width: " << rectWidth << endl;
  
}

void BarChart::drawSingleBar(double y_value){
	m->fillRectangle(cWidth/2,cHeight-40,rectWidth-20, -(cHeight-40-y_value*.9*(cHeight/5)));
}

void BarChart::drawColoredBar(double y_value){
	const Font* font = FontFace::get("TIMES", 12, FontFace::BOLD);
	Style* barStyle = new Style(font,1,0,.8,.8,.9,.99);
	const double scaleY = 1 - 40.0/cHeight;
	double FUNC2PIX = cHeight/5;
	sm->fillRectangle(cWidth/2.0-rectWidth/2.0+40,cHeight-40-1,rectWidth-40,-
                    y_value*FUNC2PIX,barStyle);
	//cout << "cWidth/2 is: " << cWidth/2 << endl;
	//sm->drawRectangle(cWidth/2, 0, 50, 50, barStyle);
}

void BarChart::drawBars(vector<double> &Y_values){
  setRectangleWidth();
	double x_pos = 40;
	const double FUNC2PIX = (cHeight-80)/5.0;
	const double scaleY = 1 - 40.0/(cHeight-40);
	for(int i = 0; i < Y_values.size(); i++){
		const Font* font = FontFace::get("TIMES", 12, FontFace::BOLD);
		float r = ((double)rand())/UINT_MAX;
		float g = ((double)rand())/UINT_MAX;
		float b = ((double)rand())/UINT_MAX;
		Style* barStyle = new Style(font,1,0,0,r,g,b);
    double barHeight = -((cHeight-80)/maxValue(Y_values)*Y_values[i]);
		m->fillRectangle(x_pos, cHeight-40, rectWidth, barHeight);//*FUNC2PIX)*1);//scaleY);	
		x_pos += rectWidth+.1*rectWidth;
	}
}
void BarChart::drawBarLabel(vector<string> &Axis_labels){
	const Font* font = FontFace::get("TIMES", 16, FontFace::BOLD);
	Style* labelStyle = new Style(font, 0, 0, 0, 0, 0, 0);
	labelStyle->setShaderIndex(CompiledWebWindow::TEXT_SHADER);
	double x_pos = 40;
		for(int i = 0; i < Axis_labels.size(); i++){
			c->addLayer(new Text(x_pos, cHeight-10, labelStyle, Axis_labels[i]));
			x_pos += rectWidth + .15*rectWidth;
		}
}
void BarChart::graphTitle(string title){
	const Font* font = FontFace::get("TIMES", 30, FontFace::BOLD);
	Style* titleStyle = new Style(font, 0, 0, 0, 0, 0, 0);
	c->addLayer(new Text(cWidth/2-100, 40, titleStyle, title));
}
