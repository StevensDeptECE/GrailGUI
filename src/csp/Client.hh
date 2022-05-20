#pragma once

#include "csp/csp.hh"

template <typename SocketImpl, typename ProtocolImpl>
class Client {
 private:
  SocketImpl s;
  ProtocolImpl p;

 public:
  /*
          Create a client which is not yet connected and
   */
  Client() : s() {}
  Client(const char addr[]) : s(addr) {}

  /*
          Make sure this client automatically flushes buffers in the
          socket and closes down everything cleanly before going away
   */
  ~Client() {}
  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;
  void connect(const char addr[]) { s.connect(addr); }

  //	void setId(UserId u) {}
  /**
           Write data into the out buffer
   */
  <T> void write(const T& data) {}

  /*
          flush the out buffer and terminate the connection
   */
  void send();

  /*
          flush the out buffer, wait until a response is received
          and then terminate the connection
  */
  void sendAndWaitforResponse();

  /*
          flush the out buffer, wait until a response is received
          and then keep the connection open for continuous replies
  */
  void sendAndKeepOpen();
};
