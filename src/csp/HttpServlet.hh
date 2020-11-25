#pragma once

#include <iostream>
#include <string>
#include <map>
#include "csp/Servlet.hh"
#include "HTTPRequest.hh"
#include "../util/Buffer.hh"

class HttpServlet : public Servlet {
public:
	HttpServlet(){};
	~HttpServlet();
    virtual void request(Buffer& out)= 0;
//	std::string getParameter(const std::string& s) const {
//		return std::string("UNIMPLEMENTED");
//	}
};
