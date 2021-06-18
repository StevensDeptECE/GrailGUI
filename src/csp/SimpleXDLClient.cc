#include "csp/csp.hh"
#include "csp/IPV4Socket.hh"
//#include <cstdlib>
#include "opengl/GLWin.hh"
#include "xdl/std.hh"
#include "xdl/XDLCompiler.hh"
using namespace std;

Log srvlog; // log all important events for security and debugging

//Signal Handlers will not work see below:
//https://stackoverflow.com/questions/51169357/unable-to-catch-sigint-sent-by-clion
//However the solution there introduces other issues so code is left as is

// NOTICE: This has been effectively replaced by GenericXDLClient
int main(int argc, char* argv[]) {
	const char* ip = argc > 1 ? argv[1] : "127.0.0.1";
	int port = argc > 2 ? atoi(argv[2]) : 8060;
	uint32_t req = argc > 3 ? atoi(argv[3]) : 1;

	GLWin::classInit();
	try {
		IPV4Socket s(ip, port);
    s.send(req);
		Buffer& in = s.getIn();

		//XDLCompiler compiler;
		//XDLType::readMeta(&compiler, in, 
    //in.displayRawRead();
//    Struct st;
//		XDLType::read(in, &st);// First read the metadata
    /*
    for (auto m : s.members)
      print out m???

    */
		//read metadata data from the incoming buffer. First metadata, then data
    /// print out

    
	} catch (const Ex& e) {
		cerr << e << '\n';
	}
	GLWin::classCleanup();
	return 0;
}
