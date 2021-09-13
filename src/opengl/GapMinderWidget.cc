#include "opengl/GapMinderWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include "data/GapMinderLoader.hh"
#include <string>
#include <iostream>
//#include <algorithm>
#include <numbers>

using namespace std;

void GapMinderWidget::init() {
	//	yAxis->init(); //minY, maxY, y+h, -h, rulerIntervalY);
	//	xAxis->init(); //minX, maxX, x, w, rulerIntervalX);

}

void GapMinderWidget::loadData(string sy, string sx, string ss, int startY, int endY){
	gml = new GapMinderLoader("res/GapMinder/GapMinderDBFile");
	
	d = gml->getDataset(sx.c_str());
	d2 = gml->getDataset(sy.c_str());
	d3 = gml->getDataset(ss.c_str());
	startYear = startY;
	endYear = endY;
}

//  void GapMinderWidget::animate(int rulerIntervalX, int rulerIntervalY){
void GapMinderWidget::update() {
	if(startYear == endYear){
		return;
	}

		// TODO: data is coming back with bad values > 1M. Why?
    x = gml->getAllDataOneYear(startYear, d);
    y = gml->getAllDataOneYear(startYear, d2);
    sizes = gml->getAllDataOneYear(startYear, d3);
		for (int i = 0; i < x.size(); i++)
      colorValues.push_back(gml->continents[i]);
#if 0
    for (int i = 0; i < x1.size(); i++){
      if(x1[i] < 1000000 && y1[i] < 1000000 && s1[i] < 1000000){
        x2.push_back(x1[i]);
        y2.push_back(y1[i]);
        s2.push_back(s1[i]);
        c2.push_back(colorContinent[gml->continents[i]]);
      }
    }
#endif
		// scale size to radius by computing sqrt of each value
		//TODO: no reason to figure out minimum square root, that should be the square root of the minimum element
		double invPi = 1.0 / numbers::pi;
		double minRadius = 1e38; // start big, find the smallest value
		for (int i = 0; i < sizes.size(); i++){
			sizes[i] = sqrt(sizes[i] * invPi); //TODO: off by factor of 2?
			if (minRadius < sizes[i])
				minRadius = sizes[i];
		}

		//TODO: Make this scale like every other axis, linear, log, etc
		double radScale = minMultiplier/minRadius;
		for (int i = 0; i < sizes.size(); i++){
			sizes[i] *= radScale;
		}

		glm::vec4 outlineColor = s->outlineColors[0]; // cycle through all colors
		
		for (int i = 0; i < x.size(); i++) {
			float xPoint = xAxis->transform(x[i]);
			float yPoint = yAxis->transform(y[i]);
		
			m->fillCircle(xPoint, yPoint, sizes[i], 3, grail::continent_colors[colorValues[i]]);
			//TODO: Make drawing edges optional? if (s->drawEdge)
			m->drawCircle(xPoint, yPoint, sizes[i], 3, outlineColor);
		}

		//TODO: put this in Axis code, unify it!
#if 0
		for (float yTick = minY; yTick <= maxY; yTick = yAxis->next(yTick)) {
			float yScreen = yAxis->transform(yTick);
			m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
			t->add(x-40, yScreen,f, (int)yTick);
		}
		for (float xTick = minX; xTick <= maxX; xTick = xAxis->next(xTick)) {
			float xScreen = xAxis->transform(xTick);
			m->drawLine(xScreen, y + h + 5, xScreen, y + h - 5, outlineColor);
			t->add(xScreen - 5, y+h+20 ,f, (int)xTick);
		}
#endif

    startYear++;
  }

