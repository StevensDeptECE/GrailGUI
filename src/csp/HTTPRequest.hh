#pragma once

#include <string>

#include "csp/Request.hh"
#include "csp/ServletMap.hh"

class HTTPRequest : public Request {
 private:
  void tokenize();
  const char* method;
  const char* url = nullptr;  // the complete url including socket number
                              // http://www.foo.com/test1.hsp
  uint32_t urlLength;
  const char* httpVersion = nullptr;
  const char* host = nullptr;
  uint32_t hostLength;
  //	const char* body;
  //	size_t bodySize;
  //	size_t bodyLength;
  // TODO: static servlet map!
  // ServletMap servlets;

 public:
  HTTPRequest(const char* ReqType);
  ~HTTPRequest();
  const static std::string POST;
  const static std::string GET;
  const static std::string UNIMPLEMENTED;

  void handle(int sckt) override;
  void handle(int sckt, const char* command) override;
};
