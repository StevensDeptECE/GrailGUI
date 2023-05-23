#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include <cstdlib>

#include "csp/CSPRequest.hh"
#include "csp/IPV4Socket.hh"
//#include "cspservlet/UtilityStructs.hh"
#include "util/List1.hh"
using namespace std;

Log srvlog;  // log all important events for security and debugging

int main(int argc, char* argv[]) {
  try {
    const char* ipaddr = argc >= 2 ? argv[1] : "127.0.0.1";
    const int port = argc >= 3 ? atoi(argv[2]) : 8000;
    Socket::classInit();
    IPV4Socket s(ipaddr, port);

    constexpr uint32_t test = 0x01020304U; // little endian 4 3 2 1
    s.send(test);
    Buffer& in = s.getIn();
    switch (test) {
      case 0: { 
        uint32_t temp = in.readU32();
        cout << temp << endl;
        break;
      }
      case 1: {
        for (uint32_t i = 0; i < 10; i++) {
          cout << in.readU32() << endl;
        }
        break;
      }
      case 2: {
        uint8_t len = (int)in.readU8();
        for (int i = 0; i < len; i++) {
          cout << in.readI8();
        }
        cout << endl;
        len = (int)in.readU8();
        for (int i = 0; i < len; i++) {
          cout << in.readI8();
        }
        cout << endl;
        // TODO: Need a way to know when @ end of buffer
        break;
      }
      #if 0
      case 4: {
        List1<uint32_t> temp = in.readList<uint32_t>();  // <List1<uint32_t>>();
        // cout<<"Used: "<<temp.getUsed()<<endl;
        // cout<<"Capacity: "<<temp.getCapacity()<<endl;
        // cout<<"Slot 0: "<<temp[0]<<endl;
        // in.read<uint32_t>();
        //TODO: Replace 15 by length of the list
        for (uint32_t i = 0; i < 15; i++) {
          // cout<<"Expected: "<<(int)in.read<uint32_t>()<<"\t\tActual:
          // "<<(int)temp[i]<<endl; cout<<in.read<uint32_t>()<<endl;

          // cout<<temp[i]<<endl;
          cout << temp.getData(i) << endl;
        }
        // cout<<sizeof(in)<<endl;

        break;
      }
      #endif
    }
  Socket::classCleanup();
  } catch (const Ex& e) {
    cerr << e << '\n';
  }
  return 0;
}
