#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include "csp/IPV4Socket.hh"
#include "csp/HTTPRequest.hh"
#include <cstdlib>
using namespace std;

Log srvlog; // log all important events for security and debugging

int main(int argc, char* argv[]) {
	try {
		IPV4Socket s(8002);
		s.attach((new HTTPRequest("SERVER")));
		s.wait(); // main server wait loop
	} catch (const Ex& e) {
		cerr << e << '\n';
	}
	return 0;
}


/*
 * With HttpServer
 *
 * *
 */
//int main(int argc, char* argv[]) {
//	HttpServer s(8002);
//	return 0;
//}
