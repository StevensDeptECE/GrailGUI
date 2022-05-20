//
// Created by dkruger on 11/01/17.
//
#pragma once

#include <csp/CSPServlet.hh>
//#include "csp/HttpServlet.hh"
//#include "csp/HTTPRequest.hh"
class CSPTest4 : public CSPServlet {
 private:
 public:
  CSPTest4() {}
  ~CSPTest4() {}
  void request(CSPRequest& r) {
    Buffer& out = r.getOut();
    uint8_t a = 1;
    uint16_t b = 2;
    uint32_t c = 3;
    uint64_t d = 12345678901234567;
    int8_t e = -1;
    int16_t f = -2;
    int32_t g = -3;
    int64_t h = -12345678901234567;
    float i = 1.234567;
    double j = 1.23456789012345;
    string s = "This is a test string.";
    out << a << b << c << d << e << f << g << h << i << j << s;
  }
};
