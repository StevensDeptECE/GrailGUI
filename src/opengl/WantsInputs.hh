#pragma once

class Canvas;

class WantsInputs {
  Canvas* mycanvas;

 public:
  WantsInputs(Canvas* c);
  virtual bool handleInput(int input) = 0;
};
