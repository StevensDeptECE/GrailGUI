#pragma once

#include <cstdint>

#include "csp/Request.hh"
#include "csp/Socket.hh"
#include "csp/SocketIO.hh"

#define MAX_REQUEST_SIZE 2047
struct client_info;
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
  IPV4Socket(uint16_t port,
             const char* host = nullptr);  // Server
  ~IPV4Socket();

  // ~IPV4Socket() {
  //   out.flush();
  //   close(sckt);
  // }
  void wait() override;
  void send(const char* command);  // For HTTP
  void send(uint32_t reqn);        // For CSP
  void sendAndAwait(uint32_t reqn, Request& r);

  std::vector<client_info> clients;
  void dropClient(struct client_info* info);
  std::string getClientAddress(struct client_info* info);
  void handleClientConnection(
      client_info* info);  // Handle requests on server-side
  static int send(socket_t sckt, const char* buf, int size, int flags);
  static int recv(socket_t sckt, const char* buf, int size, int flags);
};
