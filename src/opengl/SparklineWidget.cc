#include "opengl/SparklineWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void SparklineWidget::init() {
}

void SparklineWidget::setTitle(const string& s){
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
    t->add(x, y-10, f , s.c_str(), s.length()); 
}

void SparklineWidget::chart(const vector<double>& yLocations, 
    const vector<double>& xLocations, glm::vec4& c){

    m ->drawRectangle(x, y, w, h, grail::black);

    yAxis->init(minY, maxY, y+h, -h, 0);
    xAxis->init(minX, maxX, x, w, 0);

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    float xPoint1 = xAxis->transform(xLocations[0]);
    float yPoint1 = yAxis->transform(yLocations[0]);

    for (int i = 1; i < xLocations.size(); i++) {
      float xPoint2 = xAxis->transform(xLocations[i]);
      float yPoint2 = yAxis->transform(yLocations[i]);

      m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2, c);

      xPoint1 =xPoint2;
      yPoint1 =yPoint2;
    }

}

void SparklineWidget::setAnimation(const vector<double>& yLocations, int nPerBox, 
    int smoothness, glm::vec4& c){

  count = 0;
  lineColor = c;
  anim = yLocations;
  this->nPerBox = nPerBox;
  this->smoothness = smoothness;

  xVec = {};
  yVec = {};

}

void SparklineWidget::update(){

  if(count >= (anim.size()- nPerBox)*smoothness){
    return;
  }
  m->clear();

  if (count == 0){
    for(int i = 0; i <= nPerBox; i++){
      yVec.push_back(anim[i]);
      xVec.push_back(i);
    }
    yVec.push_back(anim[nPerBox+1]);
  }else{
    double lastXVec = xVec[1]; 
    double lastXVecEnd = xVec[nPerBox];
    for (int i = 1; i < nPerBox; i++){
      xVec[i] = xVec[i] - 1.0/smoothness;
    }

    if(count % smoothness == 1 && count != 1){
      yVec.push_back(anim[(count/smoothness) + nPerBox+1]);
      xVec.push_back(nPerBox);
      xVec.erase(xVec.begin());
      yVec.erase(yVec.begin());
    }else{
      yVec[0] = ((yVec[0]-yVec[1])*xVec[1]/lastXVec) + yVec[1];
      yVec[nPerBox]= ((yVec[nPerBox+1]-yVec[nPerBox])*(nPerBox-xVec[nPerBox-1]))+ yVec[nPerBox];
    }
    
      
  }

  m->drawCircle(x+w+50, y+h+50, 6, 5, grail::blue);
  
  chart(yVec, xVec, lineColor);

  m->update();

  count++;
}