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

//#include "csp/Request.hh"
#include "util/Buffer.hh"

#ifdef _WIN32
#include <winsock2.h>
#include "csp/Request.hh"
#endif

using namespace std;

class Request; // forward reference, all code is included in .cc
class Socket {
 protected:
  const char* address;
  uint16_t port;
  #ifdef __linux__
       char sockaddress[16]; // placeholder big enough to hold sockaddr_in structure
  #elif _WIN32
      static WSADATA wsaData;
      struct addrinfo* result;
  #endif
  Request* req;    // to be called when a request is INCOMING (req->handle() )
  Buffer in, out;  // buffers to send and receive data

 public:
  Socket(const char* addr, uint16_t port);

  // TODO: simplify
  // below two constructors could be merged depends on the location of variable
  // "req" Constructor for CSP server
  Socket(uint16_t port, Request* req);

  Socket(uint16_t port);  // Constructor for server (addres not specified)
  ~Socket();

  static void classCleanup();
  static void classInit();
  void attach(Request* r) { req = r; }

  Buffer& getOut() { return out; }
  Buffer& getIn() { return in; }
  virtual void wait() = 0;
};
