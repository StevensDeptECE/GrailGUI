#pragma once

#include <cstdint>
#include <string>

#include "csp/Request.hh"
#include "csp/Socket.hh"
#include "csp/SocketIO.hh"
/*
        IPV4Socket represents a socket connection that is portable between all
   platforms. The code must all be encapsulated in IPV4Socket.cc and hides any
   implementation details This implementation uses TCP/IP sockets. Eventually a
   corresponding class UDP4Socket will be written for faster Datagrams without
        acknowledgement and a guaranteed delivery system will have to be written
   on top of that. Header files are being moved to .cc so external code does not
   have to deal with the large includes as well as the system specific nature of
   those includes

*/
class IPV4Socket : public Socket {
 private:
  socket_t sckt;

 public:
  IPV4Socket(const char* addr, uint16_t port);  // Client
  IPV4Socket(uint16_t port);                    // Server
  ~IPV4Socket() {
    out.flush();
    close(sckt);
  }
  void listenOnPort();
  //	void send(const char data[], size_t bytes);
  //	size_t receive(char data[], size_t bytes);
  void wait();
  void send(const char* command);  // For HTTP
  void send(uint32_t reqn);        // For CSP
  static int send(socket_t sckt, const char* buf, int size, int flags);
  static int recv(socket_t sckt, const char* buf, int size, int flags);
};
