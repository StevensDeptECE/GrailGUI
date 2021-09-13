#include "csp/csp.hh"
#include "csp/IPV4Socket.hh"
#include "xdl/XDLRequest.hh"
#include "opengl/GLWin.hh"
#include <cstdlib>
using namespace std;

Log srvlog; // log all important events for security and debugging

/*
  Request 0: register my computer as a collaboration server and allow anyone
  who successfully logs in with the password to get in. 
*/
void registerCollaborationServer(Buffer& in, Buffer& out) {

}

/*
  Request 1: request to connect to collaboration server.
  Request is eventually challenged with a nonce encrypted using the sender's public key
*/
void connectToCollaborationServer(Buffer& in, Buffer& out) {

}


int main(int argc, char* argv[]) {
  int port = argc > 1 ? atoi(argv[1]) : 9910; // Collaboration server by default on port 9910
	GLWin::classInit();
	try {
		IPV4Socket s(port);
    XDLRequest req(registerCollaborationServer, connectToCollaborationServer);
		s.attach(&req);
		s.wait(); // main server wait loop
	} catch (const Ex& e) {
		cerr << e << '\n';
	}
	GLWin::classCleanup();
	return 0;
}
