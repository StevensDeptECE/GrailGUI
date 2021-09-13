#include <cstdlib>

#include "csp/IPV4Socket.hh"
#include "csp/csp.hh"
#include "opengl/GLWin.hh"
#include "csp/XDLServer.hh"
using namespace std;

class CollaborationConnectionServer : public XDLServer {
private:
  struct Party {
    uint32_t ip;
    uint16_t port;
    static Party read(Buffer& in) {
      return Party(in.readU32(, in.readU16()); // TODO: make big-endian
    }
  };
  Party sharedGrail; // the grail being shared by others
  DynArray<Party> sharers; // the ones all accessing the common grail
  /*
    Request 0: register my computer as a collaboration server and allow anyone
    who successfully logs in with the password to get in.
  */
  void registerCollaborationServer() {
    // TODO: How to read IP address portably?
    // TODO: first make this bigendian, but even then it doesn't work for IPv6
    sharedGrail = Party::read(in);
  }

  /*
    Request 1: request to connect to collaboration server.
    Request is eventually challenged with a nonce encrypted using the sender's
    public key
  */
  void connectToCollaborationServer() {
    // TODO: How to read IP address portably?
    sharers.add({Party::read(in)})

  }

  void init() {
    req.addRequest(registerCollaborationServer); // req 0
    req.addRequest(connectToCollaborationServer); // req 1
  }
};

int main(int argc, char* argv[]) {
  return CollaborationConnectionServer::initServer(argc, argv);
}
