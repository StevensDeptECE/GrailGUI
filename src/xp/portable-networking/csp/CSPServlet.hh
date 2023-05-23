#pragma once

#include <iostream>
#include <vector>

#include "csp/Request.hh"
#include "csp/Servlet.hh"

class CSPRequest;

class CSPServlet {
 private:
  static std::vector<CSPServlet*> servlets;

 public:
  ~CSPServlet();
  CSPServlet() {}

  void request(Request& r) { request((CSPRequest&)r); }
  static int add(CSPServlet* s) {
    servlets.push_back(s);
    return servlets.size() - 1;
  }

  virtual void request(CSPRequest& r) = 0;

  friend class CSPRequest;
};
