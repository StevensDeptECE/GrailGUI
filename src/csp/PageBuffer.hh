//
// Created by Class2019 on 2/21/2019.
//
#include <iostream>

class pageBuffer {
 private:
  char* content;
  uint32_t length;

 public:
  pageBuffer(char* content, uint32_t length)
      : content(content), length(length) {}
  ~pageBuffer() { delete[] content; }

  void printPage() { puts(content); }
};