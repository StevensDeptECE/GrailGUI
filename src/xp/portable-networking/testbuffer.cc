#include "csp/csp.hh"
#include <iostream>
#include <string>
#include "util/DynArray.hh"
using namespace std;
// our application errors must be defined before Ex.hh
#include <cstdlib>
#include "util/Buffer.hh"

using namespace std;

Log srvlog;  // log all important events for security and debugging

// Signal Handlers will not work see below:
// https://stackoverflow.com/questions/51169357/unable-to-catch-sigint-sent-by-clion
// However the solution there introduces other issues so code is left as is

// write a list to a buffer, receiver must know what is coming


inline void write(Buffer& b, uint32_t v) {
    b.write(v);
}

inline void write(Buffer& b, uint64_t v) {
    b.write(v);
}


void writeList(List1<uint32_t>& list, Buffer& b) {
    b.write(list.size());
    for (uint32_t i = 0; i < list.size(); i++) {
        write(b, list[i]);
    }
}

void readList(List1<uint32_t>& list, Buffer& b) {
    uint32_t len = b.read<uint32_t>();
    for (uint32_t i = 0; i < len; i++) {
        list.add(b.read<uint32_t>());
    }
}

class StockQuote {
public:
    float hi, low, open, close;
    uint32_t volume;
   friend void write(Buffer& b, const StockQuote& q) {  /// list[i].write(b);
        b.write(q.hi);
        b.write(q.low);
        b.write(q.open);
        b.write(q.close);
        b.write(q.volume);
    }    
    friend void read(Buffer& b, const StockQuote& q) {  /// list[i].write(b);
        q.hi = b.read<float>();
        q.low = b.read<float>();
        q.open = b.read<float>();
        q.close = b.read<float>();
        q.volume = b.read<uint32_t>();
    }    
};

void sendBinaryNoMetadata(Buffer& b) {
    b.write("hello", 5);
    string x = "world!";
    b.write(x);
    uint32_t y = 513; // 1 + 2*256:  0x00000201
    uint64_t z = 0x0100000005040302ULL;
    b.write(y);
    b.write(z);

    DynArray<uint32_t> list;
    list.add(3);
    list.add(1);
    list.add(4);
    list.add(1);
    writeList(list, b);
    DynArray<StockQuote> list2;
    list2.add(StockQuote{5.0, 2.0, 3.0, 4.0, 125000});
    list2.add(StockQuote{5.5, 2.8, 3.5, 4.2, 228000});
    writeList(list, b);

}

void sendBinaryWithMetadata(Buffer& b) {
    
}

int main(int argc, char* argv[]) {
  try {
    Buffer b(1024, true);


  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  return 0;
}
