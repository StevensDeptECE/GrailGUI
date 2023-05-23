#pragma once

class Combiner {
 private:
  vector<Page> pages;
  char* text;

 public:
  Combiner(const char staticFileName[]);

  void parsePage(int pageNum, const char dynamicFileName[]) {}
};
