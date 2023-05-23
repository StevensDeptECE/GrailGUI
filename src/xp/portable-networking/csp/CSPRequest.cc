#include "csp/CSPRequest.hh"

#include <csp/csp.hh>
#include <iostream>

#include "csp/CSPServlet.hh"
#if 0
#include "csp/cspservlet/CSPTest1.hh"
#include "csp/cspservlet/CSPTest2.hh"
#include "cspservlet/CSPTest3.hh"
#include "cspservlet/CSPTest4.hh"
#include "cspservlet/CSPTest5.hh"
#include "cspservlet/CSPTest6.hh"
#include "cspservlet/CSPTest7.hh"
#endif

using namespace std;

CSPRequest::CSPRequest() : Request() {
  #if 0
  CSPServlet::add(new CSPTest1());
  CSPServlet::add(new CSPTest2());
  CSPServlet::add(new CSPTest3());
  CSPServlet::add(new CSPTest4());
  CSPServlet::add(new CSPTest5());
  CSPServlet::add(new CSPTest6());
  CSPServlet::add(new CSPTest7());
  #endif
  cout << "Successfully added servlet" << endl;
}
void CSPRequest::handle(int fd) {
  cout << "begin handle" << endl;
  out.attachWrite(fd);
  in.attachRead(fd);
  in.displayRaw();
  // buffer ..   buffer+dataSize
  //  for now, hardcoded first 4 bytes of buffer are the servlet index number
  uint32_t servletId = in.readU32();
  cout << "servletId: " << servletId << '\n';
  if (servletId >= CSPServlet::servlets.size()) {
    // srvlog.error(Errcode::ILLEGAL_SERVLETID);
    // commented this line out because it causes an error
    //  ERROR: In function `CSPRequest::handle(int)':
    //  CSPRequest.cc:(.text+0xf6): undefined reference to `srvlog'
    cout << "ERROR::ILLEGAL_SERVLETID\n";
    return;
  }
  CSPServlet* csps = CSPServlet::servlets.at(servletId);
  csps->request(*this);

  out.flush();
}

CSPRequest::~CSPRequest() {}
void CSPRequest::handle(int, char const*) {}

#if 0
void CSPRequest::clientWrite(int cSock, uint32_t& x){
	out.attach(cSock);
	out << x;
	out.flush();
}

void CSPRequest::clientRead(int cSock, int verbose){
	in.attachRead(cSock);
	uint32_t x = in.read<uint32_t>();

	if(verbose == 1){
		cout<< "reply from buffer: "<< x<< "\n";
	}
}
#endif
/**
This is what CSP servlets will look like:

void request(CSPRequest& req) {
        Buffer& in = req.getInput();
        Buffer& out = req.getOutput();
        in.getUint32()
        out.write(...);
}

*/
