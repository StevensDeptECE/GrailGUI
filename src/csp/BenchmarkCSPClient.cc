#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include <cstdlib>

#include "csp/IPV4Socket.hh"
using namespace std;

Log srvlog;  // log all important events for security and debugging

const char* url;
uint32_t page;
uint32_t numTrials;

IPV4Socket s;
uint32_t bytesSent = 0;
uint32_t bytesReceived = 0;
uint32_t failCount = 0;

inline void contactOnce() {
  /*
   * create uninitialized socket because we are going to reuse
   * socket to call server repeatedly.
   * we do not want to keep re-initializing buffer objects,
   * so we have to provide a call that reconnects from the same socket
   * but the constructor should just allocate the buffers once
   */
  try {
    s.connect(url);  // break down the url into ip and port and connect to it
    Buffer& out = s.getOut();
    out.write(0);  // request page 0 from the CSP server
    bytesSent += out.used();
    out.flush();  // send the request
    Buffer& in = s.getIn();
    in.read();  // read the reply into the input buffer
    bytesRead += in.used();
  } catch (const Ex& e) {
    cerr << e << '\n';
    ++failCount;
  }
}

void connectMultipleTimes() {
  for (uint32_t t = 0; t < numTrials; ++t) {
    connectOnce();
  }
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cerr << "Usage: benchmarkcspclient url page numtrials\n";
    exit(0);
  }
  url = argv[1];
  page = atoi(argv[2]);
  numTrials = atoi(argv[3]);
  Benchmark::benchmark(connectMultipleTimes);
  return 0;
}
