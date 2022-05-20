#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include <chrono>
#include <cstdlib>

#include "csp/HTTPRequest.hh"
#include "csp/IPV4Socket.hh"
using namespace std;

Log srvlog;  // log all important events for security and debugging

int main(int argc, char* argv[]) {
  try {
    IPV4Socket s("127.0.0.1", 8002);
    s.attach(new HTTPRequest("CLIENT"));

    auto start = chrono::high_resolution_clock::now();
    s.send(
        "GET benchmark4.hsp HTTP/1.1\nHost: 127.0.0.1:8002\n");  // command
                                                                 // should end
                                                                 // with \n
    Buffer& in = s.getIn();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "\nTotal time in microseconds: " << duration.count();
  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  return 0;
}
