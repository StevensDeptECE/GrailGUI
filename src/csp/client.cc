#include "csp/CSPConfig.hh"
#include "csp/CSPRequest.hh"
#include "csp/IPV4Socket.hh"
#include "csp/Request.hh"
#include "csp/Socket.hh"

int main(int argc, char* argv[]) {
  Socket* s = new IPV4Socket("127.0.0.1", "");
  s->attach(new CSPRequest(*s));
  s->send();
  return 0;
}
