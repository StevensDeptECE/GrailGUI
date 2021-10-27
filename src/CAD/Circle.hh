#pragma once

#include "opengl/Sector.hh"

class Circle : public Sector{
private:
public:
  Circle(float x,float y,float rad,float angleInc,Style* s): 
  Sector(x,y,rad,rad,0,361,angleInc,s){}

  void findArea();
  void changeRadius(float newRadius);
  void render();

};
