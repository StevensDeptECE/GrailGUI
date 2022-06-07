#include "csp/IPV4Socket.hh"
#include "csp/XDLRequest.hh"
#include "csp/csp.hh"
#include "opengl/GLWin.hh"
#include "xdl/List.hh"
//#include "XDLServlet.hh"
#include <cstdlib>
using namespace std;

Log srvlog;  // log all important events for security and debugging

class StockQuote {
 public:
  float open, high, low, close;
  constexpr StockQuote(float open, float high, float low, float close)
      : open(open), high(high), low(low), close(close) {}
  friend void write(Buffer& buf, const StockQuote& data) {
    buf.write(data.open);
    buf.write(data.high);
    buf.write(data.low);
    buf.write(data.close);
    buf.checkAvailableWrite();
  }
  friend void writeMeta(Buffer& buf, const StockQuote& data) {
    buf.writeStructMeta("StockQuote", 4);
    buf.write(DataType::F32, "open");
    buf.write(DataType::F32, "high");
    buf.write(DataType::F32, "low");
    buf.write(DataType::F32, "close");
  }
  uint32_t fieldSize() const { return 22; }
};

int main(int argc, char* argv[]) {
  int port = argc > 1 ? atoi(argv[1]) : 8060;
  GLWin::classInit();
  try {
    IPV4Socket s(port);
    List<const StockQuote> quotes("AAPL");

    //for (int i = 0; i < 70; i++) {
     //      quotes.add(StockQuote(i, i, i, i));
    //}
    XDLRequest req(&quotes);
    s.attach(&req);
    s.wait();  // main server wait loop
  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  GLWin::classCleanup();
  return 0;
}
