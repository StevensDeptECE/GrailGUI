#pragma once

#include "csp/Request.hh"
#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "xdl/SymbolTable.hh"

class XDLCompiler;
class XDLRequest : public Request {
 private:
  DynArray<const XDLType*> xdlData;
  XDLCompiler* compiler;

 public:
  XDLRequest(const char filename[]);
  ~XDLRequest() override;
  XDLRequest(const XDLType* xdl);
  XDLRequest(const XDLRequest& r) = delete;
  XDLRequest& operator=(const XDLRequest& r) = delete;
  void addPage(const char metaDataFilename[], const char filename[]);
  void addPage(const char filename[]);
  void handle(int fd) override;
  // below is from father class Request, it is for http server
  void handle(int sckt, const char* command) override;
  // TODO:	getParameter(const string& name);
};
