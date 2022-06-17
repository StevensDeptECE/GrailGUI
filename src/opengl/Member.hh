#pragma once

class Tab;
class MainCanvas;

/**
 * @brief A component contained in a Tab
 *
 */
class Member {
 protected:
  Tab* tab;
  MainCanvas* c;
  std::string name;

 public:
  Member(Tab* tab, std::string name = "Member", double frameRate = -1,
         double dt = 0.0001);
  Member(GLWin* w, std::string name = "Member", double frameRate = -1,
         double dt = 0.0001);

  void setFrameRate(double frameRate);
  void setModelDt(double dt, double defaultDt);

  std::string getName();
  void setName(std::string newName);

  // Allow members to override init, render, and update
  // Tabs iterate through each of these manually

  virtual void init();
  virtual void render();
  virtual void update();

  Tab* getParentTab();
  MainCanvas* getParentCanvas();
};
/*
template <typename T>
class Builder {
 public:
  static T builder() { return {}; }
  T& build() { return static_cast<T&>(*this); }
};

class MemberBuilder : public Builder<MemberBuilder> {
 private:
  MemberBuilder(MemberBuilder const&) = delete;
  MemberBuilder(MemberBuilder&&) = delete;
  MemberBuilder& operator=(MemberBuilder const&) = delete;
  MemberBuilder& operator=(MemberBuilder&&) = delete;

  Tab* tab;
  double dt, defaultDt, frameRate;

 public:
  MemberBuilder(Tab* tab) :tab(tab){}
  MemberBuilder& setModelDt(double dt, double defaultDt) {}
  MemberBuilder& setFrameRate() {}
};  */