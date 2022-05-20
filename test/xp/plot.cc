#include "opengl/MultiShape2D.hh"
#include "opengl/GLWin.hh"
#include "opengl/GLMath.hh"
#include "plot.hh"
#include <algorithm>
#include <numeric>
#include <vector>
#include "opengl/Text.hh"
#include "opengl/GLWinFonts.hh"
#include <memory>
#include "opengl/Style.hh"
#include "opengl/Tab.hh"
#include <sstream>
#include <iomanip>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>
using namespace std;

PointPlot::PointPlot(shared_ptr<Tab> tab, float x, float y, float w, float h){
	const Font* font = FontFace::get("TIMES",30, FontFace::BOLD);
	srand(time(NULL));

	float r = (float)((rand()%(10-4+1) + 4) / 10.0);
	float g = (float)((rand()%(10-4+1) + 4) / 10.0);
	float b = (float)((rand()%(10-4+1) + 4) / 10.0);

	s = new Style(font, 1,0,0,0,0.3,1);
	s->setLineWidth(2);
	s->setShaderIndex(GLWin::COMMON_SHADER);
	textStyle = new Style(font,0,0,0,0,0,0);
	textStyle->setShaderIndex(GLWin::TEXT_SHADER);
	c = cw->addCanvas(s,x,y,w,h);
	m = c->addLayer(new MultiShape2D(s));
	cWidth = c->getWidth();
	cHeight = c->getHeight();	
}


void PointPlot::add_Point(float add_x, float add_y){
	X.push_back(add_x);
	Y.push_back(add_y);
}
// void pointPlot::remove_point(float x, float y){	
// 	vector<float>::iterator it_x = find(X.begin(), X.end(), x);
// 	vector<float>::iterator it_y = find(Y.begin(), Y.end(), y);
// 	int index_x = distance(X.begin(), it_x)
// 	}
void PointPlot::add_Points(vector<float> &x, vector<float> &y){
	X.insert(X.end(), x.begin(), x.end());
	Y.insert(Y.end(), y.begin(), y.end());
}
void PointPlot::linePlot(vector<float> &X, vector<float> &Y){
	if ((X.size() == Y.size())&&(X.size() > 1)) {
		for(int i = 1; i < X.size(); i++){
			m->drawLine(X[i-1]+cWidth/2, cHeight/2-Y[i-1], X[i]+cWidth/2, cHeight/2-Y[i]);
		}
	}
	else{
		//Figure out what to place here.
		//	m->drawPoint(X[i]+40, cHeight-Y[i]-40);
	}	
}
void PointPlot::scatterPlot(vector<float> &X, vector<float> &Y){
	if(X.size() == Y.size()){
		for(int i = 0; i< X.size(); i++){
			//Figure out what drawPoint is
			m->drawPoint(X[i]+cWidth/2, cHeight/2-Y[i]);
		}
	}
	else if(X.size()<Y.size()){
		for(int i = 0; i<X.size(); i++){
			//Figure out what drawPoint is
			m->drawPoint(X[i]+cWidth/2, cHeight/2-Y[i]);
		}
	}
	else{
		for(int i = 0; i<Y.size(); i++){
			//Figure out what drawPoint is
				m->drawPoint(X[i]+cWidth/2, cHeight/2-Y[i]);
		}
	}
}
float PointPlot::maxWidth(vector<float> &X) {
	float maxValue = std::numeric_limits<float>::min();
	for(int i = 0; i< X.size(); i++){
		if(X[i] > maxValue){
			maxValue = X[i];
		}
	}
	return maxValue;
}
float PointPlot::minWidth(vector<float> &X) {
	float minValue =  std::numeric_limits<float>::max();
	for(int i =0; i < X.size(); i++){
		if(X[i] < minValue){
			minValue = X[i];
		}
	}
	return minValue;
}

float PointPlot::maxHeight(vector<float> &Y){
	float maxHeight = std::numeric_limits<float>::min();
  for(int i =0; i< Y.size(); i++){
		if(Y[i] > maxHeight){
			maxHeight = Y[i];
		}
	}
	return maxHeight;
}
float PointPlot::minHeight(vector<float> &Y){
	float minHeight = std::numeric_limits<float>::max();
	for(int i = 0; i<Y.size(); i++){
		if(Y[i] < minHeight){
			minHeight = Y[i];
		}
	}
	return minHeight;
}

void PointPlot::linearRegression(vector<float> &X, vector<float> &Y){
	if(X.size() != Y.size()){
		throw "Vectors are not the same size.";
	}
	float n = X.size();	
	float X_sum = accumulate(X.begin(), X.end(), 0);
	float Y_sum = accumulate(Y.begin(), Y.end(), 0);
	cout << "X total: " << X_sum << endl;
	
	
	float X_squared = 0;
	for(int i = 0; i< X.size(); i++){
		X_squared += X[i]*X[i];
	}
	
	float Y_squared = 0;
	for(int i = 0; i< Y.size(); i++){
		Y_squared += Y[i]*Y[i];
	}
	float XY_sum = 0;
	for(int i = 0;i< X.size(); i++){
		XY_sum += X[i]*Y[i];
	}

	float a = (n * XY_sum - X_sum * Y_sum) / (n * X_squared - X_sum*X_sum);
	float b = (Y_sum - a * X_sum) / n;

	cout << "a: " << a << '\t' << "b: " << b << endl;
	
	float p1_x = 0;
	float p1_y = b;
	float p2_x = maxWidth(X);
	float p2_y = p2_x*a + b;
	m->drawLine(p1_x+cWidth/2, cHeight/2-p1_y, p2_x+cWidth/2, cHeight/2-p2_y);
}

void PointPlot::drawAxes(){
	m->drawLine(0,cHeight/2, cWidth, cHeight/2);
	m->drawLine(cWidth/2,0,cWidth/2,cHeight);
}
void PointPlot::drawTicks(){
	float x_pos = 0;
	float y_pos = 0;
	for(int i = 0; i <= 10; i++){
		m->drawLine(x_pos, cHeight/2-(.01*cHeight),x_pos,cHeight/2+(.01*cHeight));
		x_pos += cWidth/10;
	}
	for(int i = 0; i <= 10; i++){
		m->drawLine(cWidth/2-.01*cWidth,y_pos,cWidth/2+.01*cWidth,y_pos);
		y_pos += cHeight/10;
	}


}
void PointPlot::tickValues(vector<float> &X, vector<float> &Y){
	float increment_x = (maxWidth(X)+minWidth(X))/5;
	float increment_y = (maxHeight(Y)+minHeight(Y))/5;
	//	c->addLayer(new Text(5,cHeight-5,textStyle, "0,0"));
	 int tickPos_x = -10;
   int tickPos_y = cHeight-25;
   float tickNum_x = 0;
	 float tickNum_y = 0;
	


	 for(int i =0; i <= 5; i++){
		 stringstream ss;
		 ss << fixed << setprecision(1) << tickNum_x;
		 string text_x = ss.str();
		 //change r=the round function later to work for decimals. 
		 c->addLayer(new Text(tickPos_x,tickPos_y,textStyle, text_x));
		 tickNum_x += increment_x;
		 tickPos_x += cWidth/5;
	 }
  for(int i =0; i <= 5; i++){
		 stringstream ss;
		 ss << fixed << setprecision(1) << tickNum_y;
		 string text_y = ss.str();
		 //change r=the round function later to work for decimals. 
		 c->addLayer(new Text(0,tickPos_y,textStyle, text_y));
		 tickNum_y += increment_y;
			 tickPos_y -= cHeight/5;
	 }
}
void PointPlot::chartTitle(string title){
	const Font* font = FontFace::get("TIMES",30, FontFace::BOLD);
  Style* titleStyle = new Style(font,0,0,0,0,0,0);
	titleStyle->setShaderIndex(CompiledWebWindow::TEXT_SHADER);
	c->addLayer(new Text(cWidth/2-150, 40,titleStyle, title));
}

void PointPlot::drawFunc(double x_min, double x_max,
												 double y_min, double y_max,
												 FuncOneVar f,
												 uint32_t pixXMin, uint32_t pixXMax,
												 uint32_t pixYMin, uint32_t pixYMax, uint32_t numPoints){
	const double x_step = (x_max-x_min)/numPoints;
		
	vector<float> X_points{};
	vector<float> Y_points{};
	int y_pixel = 0;
	double y_func = 0;
  double ratioX = pixXMax/x_max;
	double ratioY = pixYMax/y_max;
	for (double i = x_min; i <= x_max; i+= x_step) {
	 	y_func = f(i);
	 	X_points.push_back(i*ratioX);
		y_pixel = y_func*ratioY + pixYMin;
		Y_points.push_back(y_pixel);
	}
	linePlot(X_points, Y_points);
}
void PointPlot::drawTwoFunc(double val_min, double val_max,
														double step,
 														FuncOneVar f, FuncOneVar g,
														int scaleF, int scaleG)
{
	vector<float> Points_X{};
	vector<float> Points_Y{};
	double x_func = 0;
	double x_pixel = 0;
	double FUNC2PIX_X = cWidth/val_max;
	for(double i = val_min; i <= val_max; i += step){
		x_func = f(scaleF*i);
		x_pixel = x_func*FUNC2PIX_X;
		Points_X.push_back(x_pixel);
	}
	double y_func = 0;
	double y_pixel = 0;
	double FUNC2PIX_Y = cHeight/val_max;
	for (double i = val_min; i <= val_max; i += step){
		y_func = g(scaleG*i);
		y_pixel = y_func*FUNC2PIX_Y;
		Points_Y.push_back(y_pixel);
  }
	linePlot(Points_X, Points_Y);
}
//following method will only work for months as julian calendar is not fully working
//pointPlot::monthPlot(vector<float> date, vector<float> price){
//}
