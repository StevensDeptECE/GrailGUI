#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include <cstdlib>

#include "csp/CSPConfig.hh"
#include "csp/CSPRequest.hh"
#include "csp/HTTPRequest.hh"
#include "csp/IPV4Socket.hh"
#include "csp/Request.hh"
#include "csp/Socket.hh"
using namespace std;

Log srvlog;  // log all important events for security and debugging

CSPConfig conf;  // create a config object

void mainWait() {
  char* homeDir = getenv("HOME");  //  /home/dkruger on mac: /Users/userid
  string baseDir = homeDir;
  baseDir += "/.csp/";
  const char* cspconfig = getenv("CSPCONFIG");
  string confFile =
      baseDir +
      (cspconfig == nullptr
           ? ""
           : cspconfig);  // yourhomedir/.csp/ contents of CSPCONFIG variable
  cout << confFile.c_str() << "\n";
#if 0
	conf.load(confFile.c_str());
	cout << "Log file=" << conf.getString("logfile") << '\n';
	srvlog.setLogFile("test.log");
	srvlog.setLevel((Log::Level)conf.getUInt32("loglevel"));
	srvlog.warn(Errcode::FILE_NOT_FOUND);
	srvlog.error(Errcode::CERTIFICATE_EXPIRED);

 	const string protocol = conf.getString("protocol");
	const string handler = conf.getString("handler");
	const int port = conf.getUInt16("port");
#endif

  const string protocol = "ipv4";
  const string handler = getenv("SERVERTYPE");

  const char* connection = "127.0.0.1";  // localhost by default
  Socket* s;
  // Server* s;
  if (protocol == "ipv4" && handler == "http") {
    s = new IPV4Socket(connection);
    s->attach(new HTTPRequest());
    // s = new Server<IPV4Socket, HTTPRequest>(port);
  } else if (protocol == "ipv4" && handler == "csp") {
    s = new IPV4Socket(connection);
    s->attach(new CSPRequest());
    // s = new Server<IPV4Socket,CSPRequest>(port);

  } else if (protocol == "ipv6" && handler == "http")
    ;  //		s = new IPV6Socket<HTTPequest>(connection);
  else if (protocol == "ipv6" && handler == "csp")
    ;  //		s = new IPV6Socket<CSPRequest>(connection);
  else
#if 0
  if (protocol == "udp" && handler == "csp")
		;
	if (protocol == "ssl" && handler == "http")
		s = new SSLSocket<HTTPRequest>;
	if (protocol == "ssl" && handler == "csp")
		s = new SSLSocket<CSPRequest>;
	else
#endif
  {
    throw FatalEx(__FILE__, __LINE__, Errcode::BAD_PROTOCOL);
  }
  if (s != nullptr) s->wait();  // main server wait loop
}

int main(int argc, char* argv[]) {
  while (true) {
    try {
      mainWait();
    } catch (const char* msg) {
      cerr << msg << '\n';
    } catch (const Ex& ex) {
      cerr << ex << '\n';
    }
  }
  return 0;
}
