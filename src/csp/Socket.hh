#pragma once
/**
         Represent a client/server socket pair
         and call a templated handler.
         TODO: For now the handle is a pointer, not templated.  This is slightly
   larger and slower but only by one memory access

         This is envisioned to eventually support the following:

         Protocols: TCP/IP V4, IPV6, UDP, SSL

         The socket base class contains an inline buffer, used to store data
   read from the socket The socket should be agnostic as to the kind of message
   being sent.  The protocol is delegated to the Request class.

         Handlers: HTTP, HTTPS (TLS), CSP, CSPTLS, HTTPMultithreaded
         @author: Dov Kruger
 */
#include <sys/types.h>
#include <unistd.h>
#if WINDOWS


#else
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
//#include <sys/wait.h>
#endif

#include <signal.h>

#include "csp/Request.hh"
#include "util/Buffer.hh"

using namespace std;

class Socket {
 protected:
  const char* address;
  uint16_t port;

  struct sockaddr_in sockaddress;
  Request* req;    // to be called when a request is INCOMING (req->handle() )
  Buffer in, out;  // buffers to send and receive data

 public:
  Socket(const char* addr, uint16_t port)
      : address(addr),
        port(port),
        req(nullptr),
        in(BUFSIZE, false),
        out(BUFSIZE, true) {}

  // below two constructors could be merged depends on the location of variable
  // "req" Constructor for CSP server
  Socket(uint16_t port, Request* req)
      : port(port), req(req), in(BUFSIZE, false), out(BUFSIZE, true) {}
  // Constructor for HTTP server
  Socket(uint16_t port)
      : address(nullptr),
        port(port),
        req(nullptr),
        in(BUFSIZE, false),
        out(BUFSIZE, true) {}

  ~Socket() {
    if (req != nullptr) delete req;
  }

  void attach(Request* r) { req = r; }

  Buffer& getOut() { return out; }
  Buffer& getIn() { return in; }
  virtual void wait() = 0;
};
