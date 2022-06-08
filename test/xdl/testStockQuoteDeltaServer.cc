#include "csp/IPV4Socket.hh"
#include "csp/csp.hh"
#include "opengl/GLWin.hh"
#include "xdl/XDLCompiler.hh"
#include "csp/XDLRequest.hh"
//#include "xdl/std.hh"
#include "QuoteTable.hh"
using namespace std;


/*
  Server to update clients requesting repeated quote by incrementally sending just the updates.

*/
int main(int argc, char* argv[]) {
  // server runs on this ip (not specified)
  // specifies port number
  int port = argc > 1 ? atoi(argv[2]) : 8060;
  // client specifies symbol name and perhaps later, we can accept that as a parameter. Right now we are hardcoded to AAPL
  const char* symbolName = "AAPL";
  try {
    GLWin::classInit();
    List<Quote>* quotes = Quote::loadASCII("AAPL", "res/aapl.txt");
    IPV4Socket s(port);
    XDLRequest req(quotes);
    s.attach(&req);
    s.wait();  // main server wait loop
    cerr << "Read " << quotes->size() << "delta stock quotes from server\n";
  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  GLWin::classCleanup();
  return 0;
}
