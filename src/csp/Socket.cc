#include "csp/Socket.hh"

#include "csp/Request.hh"

Socket::Socket(uint16_t port, Request *req)
    : port(port), req(req), in(BUFSIZE, false), out(BUFSIZE, true) {}

Socket::Socket(const char *addr, uint16_t port)
    : address(addr),
      port(port),
      req(nullptr),
      in(BUFSIZE, false),
      out(BUFSIZE, true) {}

Socket::Socket(uint16_t port)
    : address(nullptr),
      port(port),
      req(nullptr),
      in(BUFSIZE, false),
      out(BUFSIZE, true) {}

Socket::~Socket() {
  if (req != nullptr) delete req;
}
