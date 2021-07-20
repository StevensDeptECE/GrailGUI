#pragma once

class Canvas;

class KeyReceiver {
 public:
  KeyReceiver(Canvas* c);
  virtual void handleInput(int key, int action, int mods) = 0;
};
