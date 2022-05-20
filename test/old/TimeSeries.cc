#include "TimeSeries.hh"
#include "util/JulianDate.hh"
#include "opengl/GLWin.hh"
#include "opengl/GLMath.hh"
#include "plot.hh"
#include <algorithm>
#include <numeric>
#include <vector>
#include "opengl/MultiShape2D.hh"
#include "opengl/Text.hh"
#include "opengl/GLWinFonts.hh"
#include <memory>
#include <sstream>
#include <iomanip>
#include <string>
#include <climits>
#include "opengl/Style.hh"
#include "opengl/Tab.hh"
#include <string>
#include <math.h>
using namespace std;

TimeSeries::TimeSeries(shared_ptr<CompiledWeb> cw, float x, float y, float w, float h){
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
  srand(time(NULL));

}
void TimeSeries::addPoints(vector<double> &y_values, vector<string> &time_values){
  point p;
  vector<double> Julian_Nums = setJulianDates(time_values);
  for(int i = 0; i < y_values.size(); i++){
    Points.push_back(p = {y_values[i], Julian_Nums[i]});
  }
}
// void TimeSeries::sortPoint(){
//   for(int i = 0; i < Julian_Numbers.size(); i++){
    


//   }
// }
  
void TimeSeries::drawAxes(){
  m->drawLine(40,40,40,cHeight-40);
  m->drawLine(40,cHeight-40,cWidth,cHeight-40);
}

vector<double> TimeSeries::setJulianDates(vector<string> time_values){
  string s;
  int slash_pos;
  int colon_pos;
  string month;
  string day;
  string year;
  uint32_t J_year;
  uint32_t J_month;
  uint32_t J_day;
  vector<double> Julian_Numbers;
  for(int i = 0; i < time_values.size(); i++){
    s = time_values[i];
    slash_pos = s.find("/");
    if(slash_pos == string::npos){
      break;
    }
    month = s.substr(0,slash_pos);
    s.erase(0,slash_pos+1);
    slash_pos = s.find("/");
    day = s.substr(0,slash_pos);
    s.erase(0,slash_pos+1);
    year = s;
    J_year = stoi(year);
    J_month = stoi(month);
    J_day = stoi(day);
    JulianDate j (J_year, J_month, J_day);
    Julian_Numbers.push_back(j.getDate());
  }
  for(int i = 0; i < time_values.size(); i++){
    s = time_values[i];
    colon_pos = s.find(":");

    if(colon_pos == string::npos){
      break;
    }
    month = s.substr(0,colon_pos);
    s.erase(0,colon_pos+1);
    colon_pos = s.find(":");
    day = s.substr(0,colon_pos);
    s.erase(0,colon_pos+1);
    year = s;
    J_year = stoi(year);
    J_month = stoi(month);
    J_day = stoi(day);
    JulianDate j (J_year, J_month, J_day);
    Julian_Numbers.push_back(j.getDate());
  }
  return Julian_Numbers;
}
double TimeSeries::maxYValue(){
  double maxValue = std::numeric_limits<float>::min();
  for(int i = 0; i < Points.size(); i++){
    if(Points[i].value > maxValue){
      maxValue = Points[i].value;
    }
  }
  return maxValue;
}
double TimeSeries::minYValue(){
  double minValue = std::numeric_limits<float>::max();
  for(int i = 0; i < Points.size(); i++){
    if(Points[i].value < minValue){
      minValue = Points[i].value;
    }
  }
  return minValue;
}

double TimeSeries::minJulianNumber() {
  double minValue = std::numeric_limits<float>::max();
  for(int i = 0; i < Points.size(); i++){
    if(Points[i].time < minValue){
      minValue = Points[i].time;
    }
  }
  return minValue;
}
double TimeSeries::maxJulianNumber() {
  double maxValue = std::numeric_limits<float>::min();
  for(int i = 0; i < Points.size(); i++){
    if(Points[i].time > maxValue){
      maxValue = Points[i].time;
    }
  }
  return maxValue;
}

                                 
void TimeSeries::weekLinePlot(){
  double FUNC2PIX_x =  ((double)(cWidth-40))/(Points.size()-1);
  double FUNC2PIX_y = ((double)(cHeight-80))/maxYValue();
  for (int i = 0; i < Points.size(); i++){
    m->drawLine(40+i*FUNC2PIX_x, cHeight-40-Points[i].value*FUNC2PIX_y,
                40+(i+1)*FUNC2PIX_x, cHeight-40-Points[i+1].value*FUNC2PIX_y); 
  }
}



// void TimeSeries::drawTicks(uint32_t number_ticks){
//   num_ticks = number_ticks;
//   uint32_t diff_x = (cWidth-40)/num_ticks;
//   int x_pos = 40;
//   for(int i = 0; i <= num_ticks; i++){
//     m->drawLine(x_pos,cHeight-30, x_pos, cHeight-50);
//     x_pos += diff_x;
//   }
//   int y_pos = cHeight - 40;
//   uint32_t diff_y = (cHeight-80)/num_ticks;
//   for(int i = 0; i <= num_ticks; i++){
//   	m->drawLine(30,y_pos,50,y_pos);
//   	y_pos -= diff_y;
//   }
// }
void TimeSeries::labelDates(){
  string month;
  string day;
  string year;
  string month_day;
  int x_pos = 40;
  uint32_t diff_x = (cWidth-40)/num_ticks;
  const Font* font = FontFace::get("TIMES", 12, FontFace::BOLD);
	Style* labelStyle = new Style(font, 0, 0, 0, 0, 0, 0);
  for(int i = 0; i <= num_ticks; i++){
    month = JulianDate::getMonthName(Points[i].time);
    day = to_string(JulianDate::getDay(Points[i].time));
    c->addLayer(new Text(x_pos, cHeight-20, labelStyle,month));
    c->addLayer(new Text(x_pos, cHeight-10, labelStyle,day));
    x_pos += diff_x;
  }
}

void TimeSeries::labelTicks(uint32_t number_ticks){
  num_ticks = number_ticks;
	double data_range = maxYValue()-minYValue();
	double exponent = floor(log10(data_range));
  
	double fraction_part = data_range/(pow(10,exponent));
	double new_fraction_part;

	if (fraction_part <= 1){
		new_fraction_part = 1;
	}	
	else if(fraction_part <= 2){
		new_fraction_part = 2;
	}
	else if(fraction_part <=5){
		new_fraction_part = 5;
	}
	else{
		new_fraction_part = 10;
	}

	double intermediate_range = new_fraction_part*(pow(10,exponent));
 	double intermediate_tick_step = intermediate_range/(num_ticks-1);
	double tick_exponent = floor(log10(intermediate_tick_step));

	double tick_fraction_part = intermediate_tick_step/(pow(10, tick_exponent));
	uint32_t new_tick_fraction_part;

	if(tick_fraction_part < 1.5){
		new_tick_fraction_part = 1;
	}
	else if(tick_fraction_part < 3){
		new_tick_fraction_part = 2;
	}
	else if(tick_fraction_part < 7){
		new_tick_fraction_part = 5;
	}
	else{
		new_tick_fraction_part = 10;
	}

	uint32_t nice_tick_increment = new_tick_fraction_part*(pow(10, tick_exponent));
  
	uint32_t nice_min = floor(minYValue()/nice_tick_increment)*nice_tick_increment;
	uint32_t nice_max = ceil(maxYValue()/nice_tick_increment)*nice_tick_increment;


	double FUNC2PIX_y = (cHeight-80)/maxYValue();
	double y_pos = nice_min;

  const Font* font = FontFace::get("TIMES", 12, FontFace::BOLD);
  tickStyle = new Style(font, .1, .5, 0, 0, 0, 0);
	tickStyle->setShaderIndex(CompiledWebWindow::TEXT_SHADER);
  
	for(int i = 0; i < num_ticks; i++){
		c->addLayer(new Text(5,cHeight-40-y_pos*FUNC2PIX_y+15,tickStyle, to_string(uint32_t(y_pos))));
		m->drawLine(30, cHeight-40-y_pos*FUNC2PIX_y, 50, cHeight-40-y_pos*FUNC2PIX_y);
    y_pos += nice_tick_increment;
	}
}

void TimeSeries::graphTitle(string title){
  const Font* font = FontFace::get("TIMES", 16, FontFace::BOLD);
  titleStyle = new Style(font, .1, .5, 0, 0, 0, 0);
	titleStyle->setShaderIndex(CompiledWebWindow::TEXT_SHADER);
  c->addLayer(new Text(cWidth/2-20,20,titleStyle, title));
}

void TimeSeries::plotPoints(){
  double FUNC2PIX_x =  ((double)(cWidth-40))/(Points.size()-1);
  double FUNC2PIX_y = ((double)(cHeight-80))/maxYValue();
  double x_start = Points[0].time;
  for (int i = 0; i < Points.size(); i++){
    //Fix it by doing ration of maximum x or something. Maybe diff in Julian Numbers.
    m->fillCircle(40+(Points[i].time-x_start)*FUNC2PIX_x,cHeight-40-Points[i].value*FUNC2PIX_y,3,1);
  }
}

void TimeSeries::addCandleSticks(vector<string> Times, vector<double> Opens, vector<double> Closes){
  candlestick c;
  vector<double> Julian_Numbers = setJulianDates(Times);
  if(Times.size() == Opens.size() && Opens.size() == Closes.size()){
    for(int i = 0; i < Opens.size(); i++){
      CandleSticks.push_back(c = {Julian_Numbers[i], Opens[i], Closes[i]});
    }
  }
}
double TimeSeries::maxClose(){
  double max_close = std::numeric_limits<float>::min();
  for(int i = 0; i < CandleSticks.size(); i++){
    if(CandleSticks[i].close > max_close){
      max_close = CandleSticks[i].close;
    }
  }
  return max_close;
}

void TimeSeries::drawCandleStick(candlestick c){
	const Font* font = FontFace::get("TIMES", 12, FontFace::BOLD);
	Style* upStyle = new Style(font, .5, 0, .8, 0, 0, 0);
	Style* downStyle = new Style(font, 0, 0, 0, 0, 0, 0);
	double xpos;
	double ypos;
  //Must fix this later on to not end at canvas width.

  
  double FUNC2PIX_x = ((cWidth-40)-5*CandleSticks.size())/CandleSticks.size();
  cout << "FUNCPIX_X: " << FUNC2PIX_x << endl;
  double FUNC2PIX_y = (cWidth-80)*.01;
  cout << "FUNC2PIX_y: " << FUNC2PIX_y << endl;
  double spacer = 30;

  
	if(c.close > c.open){
		xpos = (c.day-CandleSticks[0].day)*FUNC2PIX_x;
    cout << "XPOS: " << xpos << endl;
		ypos = c.open*FUNC2PIX_y;
    cout << "YPOS: " << ypos << endl;
		sm->fillRectangle(40+xpos+spacer, cHeight-40-ypos, 20,5, upStyle);
    cout << "Height: " << c.close*FUNC2PIX_y-ypos << endl;
	}
	if(c.close < c.open){
		xpos = (c.day-CandleSticks[0].day)*FUNC2PIX_x;
		ypos = c.open*FUNC2PIX_y;
    
		sm->fillRectangle(40+xpos+spacer, cHeight-40-ypos, 20, -5, downStyle);
    cout << "40 + xpos + spacer: " << 40+xpos+spacer << endl;
    cout << "cHeight-40-ypos: " << cHeight-40-ypos << endl;
	}
}
void TimeSeries::plotCandleStick(){
  for(int i = 0; i < CandleSticks.size(); i++){
    drawCandleStick(CandleSticks[i]);
  }
  cout << "SIZE CANDLESTICKS: " << CandleSticks.size() << endl;
}
