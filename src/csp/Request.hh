#pragma once
/**
   Abstract class for all protocols handled.  Each must have a handle
   method that reads from the bytes sent by the socket object and decides what
   to do.

   @author: Dov Kruger
 */
#include <cstdlib>
#include <string>
#include "csp/Socket.hh"

#include "util/Buffer.hh"

constexpr size_t BUFSIZE = 1500 * 30;
// TODO csp18summer:remove out/in, "this" in/out could be replaced by socket's
// in/out remove in,out,getOut,getIn, change handle(int fd) to handle(int fd,
// Buffer &in, Buffer &out)
class Request {
 protected:
 Socket* sckt; // Socket used to send and receive requests

 public:
  Request(Socket* sckt) : sckt(sckt) {}
  virtual ~Request() = 0;
  virtual void handle(int sckt) = 0;
  virtual void handle(int sckt, const char* command) = 0;
  Buffer& getOut()  { return sckt->getOut(); }
  Buffer& getIn()  { return sckt->getIn(); }
};
