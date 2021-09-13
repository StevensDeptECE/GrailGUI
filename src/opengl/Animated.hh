#pragma once

#include <string>
#include <cstdint>

class Tab;
class MainCanvas;

/**
 * @brief A component contained in a Tab
 *
 */
class Animated {
 protected:
  Tab* tab;
  MainCanvas* c;

 public:
  Animated(Tab* tab, double frameRate = -1, double dt = .0001);
	Animated(Tab* tab, const std::string& title, double frameRate, double dt);
	Animated(Tab* tab, const std::string& title, uint32_t w, uint32_t h, double frameRate, double dt);

  // Allow members to override init, render, and update
  // Tabs iterate through each of these manually

  virtual void init();
  virtual void render();
  virtual void update();

  Tab* getParentTab();
  MainCanvas* getParentCanvas();
};
