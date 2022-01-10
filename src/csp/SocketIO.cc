#include "csp/SocketIO.hh"

#include <cstdint>

#include "csp/csp.hh"

// TODO: Look into logging WSAGetLastError and strerror(errno)
// Using the logging object in csp.hh?
namespace SocketIO {
ssize_t send(socket_t sckt, const char *buf, int size, int flags) {
  ssize_t bytesSent = 0;
  if ((bytesSent = ::send(sckt, buf, size, flags)) == err_code) {
    if (errno != EBADF) {
      throw Ex1(Errcode::SOCKET_SEND);
    }
    perror("Warning on send(): ");
  }
  return bytesSent;
}

ssize_t recv(socket_t sckt, char *buf, int size, int flags) {
  ssize_t bytesRecv = 0;
  if ((bytesRecv = ::recv(sckt, buf, size, flags)) == err_code) {
    if (errno != EBADF) {
      throw Ex1(Errcode::SOCKET_SEND);
    }
    perror("Warning on recv()");
  }
  return bytesRecv;
}
}  // namespace SocketIO