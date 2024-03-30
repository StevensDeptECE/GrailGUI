#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include <cstdlib>

#include "csp/CSPServlet.hh"
#include "csp/CSPRequest.hh"
#include "csp/IPV4Socket.hh"
using namespace std;

Log srvlog;  // log all important events for security and debugging

// Signal Handlers will not work see below:
// https://stackoverflow.com/questions/51169357/unable-to-catch-sigint-sent-by-clion
// However the solution there introduces other issues so code is left as is

int main(int argc, char* argv[]) {
  try {
    Socket::classInit();
    IPV4Socket s(8000);
    CSPRequest req;
    s.attach(&req);
    s.wait();  // main server wait loop
    Socket::classCleanup();
  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  return 0;
}
