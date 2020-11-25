#pragma once

#include "csp/Request.hh"
#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "xdl/SymbolTable.hh"

class XDLCompiler;
class XDLRequest : public Request {
private:
  DynArray<SymbolTable*> xdlData;
  XDLCompiler* compiler;
public:
	XDLRequest(const char filename[]);
  ~XDLRequest()override;
	void handle(int fd) override;
	//below is from father class Request, it is for http server
    void handle(int sckt, const char* command) override;
	//TODO:	getParameter(const string& name);
};
