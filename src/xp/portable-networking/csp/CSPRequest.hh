#pragma once

#include "csp/Request.hh"
#include "util/Buffer.hh"

class CSPRequest : public Request {
 private:
  //	CSPServlet servlets;
 public:
  CSPRequest();
  ~CSPRequest() override;
  void handle(int fd) override;
  // below is from father class Request, it is for http server
  void handle(int sckt, const char* command) override;
  // TODO:	getParameter(const string& name);
};
