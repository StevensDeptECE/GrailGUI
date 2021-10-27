#pragma once

#include "opengl/Shape2D.hh"

class Rectangle : public Shape2D{
private:
  float width,height;
public:
  Rectangle(Canvas* c, float x, float y, float width,float height, Style* s) :
   Shape2D(c, x,y,s) {
    addPoint(x,y);
    addPoint(x,y-height);
    addPoint(x+width,y-height);
    addPoint(x+width,y);

    for(int i = 0;i<vertices.size();i++){
      lineIndices.push_back(i);
      pointIndices.push_back(i);
    }

    solidIndices.push_back(0);
    solidIndices.push_back(1);
    solidIndices.push_back(2);

    solidIndices.push_back(3);
    solidIndices.push_back(0);
    solidIndices.push_back(2);
  }

  void initIndices();
  void render();
  float getArea();
};
