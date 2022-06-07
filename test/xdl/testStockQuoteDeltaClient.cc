#include "csp/IPV4Socket.hh"
#include "csp/csp.hh"
#include "opengl/GLWin.hh"
#include "xdl/XDLCompiler.hh"
#include "xdl/std.hh"
#include "QuoteTable.hh"
using namespace std;


/*
  Demonstrate the performance of going out and getting XDL binary data for stock quotes
  
   the ability to send a request to an XDL
  server, and get in response metadata and then data By reading the metadata,
  the client can then print out ASCII views of the data.

  This is a standalone demo that does not require Grail graphics.
  The next step after this is to display the data directly on a Grail window

  Known issues at this time:
  1. Not all data types are implemented yet, ie don't send JPEG or BLOB.
   We focus mostly on numbers, strings, lists and struct for proof of concept.

  2. Formatting is very ad hoc and not efficient. We need to come up with a way
  to define good formatting that is fast.  C++ recently added format, we should
  take a look

*/
int main(int argc, char* argv[]) {
  const char* ip = argc > 1 ? argv[1] : "127.0.0.1";
  int port = argc > 2 ? atoi(argv[2]) : 8060;
  const char* symbolName = argc > 3 ? argv[3] : "AAPL";
  uint32_t roundTrips = argc > 4 ? atoi(argv[4]) : 100;
  uint32_t req = 0;
  GLWin::classInit();
  List<Quote> quotes("generic"); // create a list of quotes. We don't know what is coming.
  #if 0
   // calendar.getUpdate(); // potentially lots of objects requesting updates?
//  then send single request to get it done      
      //       s.send(req); // TODO: we need to send parameter! For now, server will send first 7800 quotes the first time, then one each time after
//    in.displayRawRead();
      XDLCompiler compiler(""); //TODO: can we reuse the compiler? 
      SymbolTable* st = new SymbolTable(&compiler);
//      quotes.read(in);
#endif
  try {
    IPV4Socket s(ip, port);
    Buffer& in = s.getIn(); // TODO: do we have to get a new input buffer every time?
    for (uint32_t trials = 0; trials < roundTrips; trials++) {
      quotes.getUpdate(in);

//      quotes.addToUpdateRequest(); // add to current update but don't send it out, batch together with others
//      calendar.getUpdate();
     
      cerr << "Read " << quotes.size() << "delta stock quotes from server\n";
    }
  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  GLWin::classCleanup();
  return 0;
}
