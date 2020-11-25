#include "csp/csp.hh"
#include "csp/IPV4Socket.hh"
//#include <cstdlib>
#include "xdl/std.hh"
#include "xdl/XDLCompiler.hh"
using namespace std;

Log srvlog; // log all important events for security and debugging

/*
  This generic client demonstrates the ability to send a request to an XDL server,
  and get in response metadata and then data
  By reading the metadata, the client can then print out ASCII views of the data.

  This is a standalone demo that does not require Grail graphics.
  The next step after this is to display the data directly on a Grail window

  Known issues at this time:
  1. Not all data types are implemented yet, ie don't send JPEG or BLOB.
   We focus mostly on numbers, strings, lists and struct for proof of concept.

  2. Formatting is very ad hoc and not efficient. We need to come up with a way to define
  good formatting that is fast.  C++ recently added format, we should take a look

*/
int main(int argc, char* argv[]) {
	const char* ip = argc > 2 ? argv[1] : "127.0.0.1";
	int port = argc > 3 ? atoi(argv[2]) : 8000;
	uint32_t req = argc > 4 ? atoi(argv[3]) : 0;
  XDLType::classInit();
	try {
		IPV4Socket s(ip, port);
    s.send(req);
		Buffer& in = s.getIn();
    in.displayRawRead();
    XDLCompiler compiler("");
    SymbolTable st(&compiler);
    st.readMeta(in);
    Buffer out("client.txt", 32768);

    const Struct* root = (Struct*)st.getRoot();
  // dump all metadata, whether used in the data or not
    for (int i = 0; i < root->getMemberCount(); i++)
      cout << root->getMemberName(i) << '\t';
    cout << '\n';

    root->display(in, out);
    out.displayText(cout);
	} catch (const Ex& e) {
		cerr << e << '\n';
	}
  XDLType::classCleanup();
	return 0;
}
