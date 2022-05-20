#pragma once
/**

         Base class for servlet
         This is the common behavior between protocols such as HTTP and CSP
         so this is the low-level data methods.

         All servlets support a request, which is a client sending a message to
   the server requesting data.  In HTTP, a request is ASCII with many potential
         lines if input, whereas in CSP it is a lot shorter, fixed format, and
   binary

         @author: Dov Kruger

*/
#include "util/Buffer.hh"
class Servlet {
 private:
 public:
  Servlet() {}
  virtual void request(Buffer& out) = 0;
  ~Servlet();
};
