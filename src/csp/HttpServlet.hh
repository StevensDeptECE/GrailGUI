#pragma once

#include <iostream>
#include <map>
#include <string>

#include "../util/Buffer.hh"
#include "HTTPRequest.hh"
#include "csp/Servlet.hh"

class HttpServlet : public Servlet {
 public:
  HttpServlet(){};
  ~HttpServlet();
  virtual void request(Buffer& out) = 0;
  //	std::string getParameter(const std::string& s) const {
  //		return std::string("UNIMPLEMENTED");
  //	}
};
