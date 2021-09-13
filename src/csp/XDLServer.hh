#pragma once

#include "opengl/GLWin.hh"
#include "csp/XDLRequest.hh"
#include <cstdint>

class XDLServer {
protected:
	IPV4Socket s;
  XDLRequest req;
  Log srvlog; // log all important events for security and debugging
public:
  static int initServer(XDLServer* xdlServer, int argc, char* argv[]) {
  	try {
      int port = argc > 1 ? atoi(argv[1]) : 9910; // Collaboration server by default on port 9910
	    GLWin::classInit();
      xdlServer->init();
    	GLWin::classCleanup();
	  }  catch (const Ex& e) {
		  cerr << e << '\n';
      return -1;
    }
    return 0;
  }
  virtual void init() = 0;

  XDLServer(uint16_t port) : s(port), req() {
		s.attach(&req);
		s.wait(); // main server wait loop
  }

};