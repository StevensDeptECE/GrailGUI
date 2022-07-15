#include "csp/Socket.hh"
#include "csp/Request.hh"
Socket::Socket(const char *addr, uint16_t port)
    : address(addr),
      port(port),
      in(BUFSIZE, false),
      out(BUFSIZE, true),
      req(nullptr) {}

Socket::Socket(uint16_t port)
    : address(nullptr),
      port(port),
      in(BUFSIZE, false),
      out(BUFSIZE, true),
      req(nullptr) {}

Socket::~Socket() {}
