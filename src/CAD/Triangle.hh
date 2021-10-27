#pragma once

#include "opengl/Shape2D.hh"

class Triangle : public Shape2D{
private:
  float base, height;
  float x2,y2,x3,y3;
public:
  Triangle(Canvas* c, float x, float y, float base, float height, Style* s) :
    Shape2D(c, x,y,s){
      addPoint(x,y);
      addPoint(x+base,y);
      addPoint(x,y+(base/2));
      for(int i = 0;i<vertices.size();i++){
        lineIndices.push_back(i);
        pointIndices.push_back(i);
      }

      solidIndices.push_back(0);
      solidIndices.push_back(1);
      solidIndices.push_back(2);
      solidIndices.push_back(0);
    }
    void initIndices();
    void render();
    void getArea(base, height);
public: 
  Triangle(Canvas* c, float x, float y, float x2, float y2,float x3, float y3,Style* s) :
    Shape2D(c, x, y, s){
      addPoint(x,y);
      addPoint(x2,y2);
      addPoint(x3,y3);
      for(int i = 0;i<vertices.size();i++){
        lineIndices.push_back(i);
        pointIndices.push_back(i);
      }

      solidIndices.push_back(0);
      solidIndices.push_back(1);
      solidIndices.push_back(2);
      solidIndices.push_back(0);

      
    }
    void initIndices();
    void render();
    void getArea(x, y, x2, y2, x3, y3);
};

