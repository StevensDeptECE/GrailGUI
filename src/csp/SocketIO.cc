#include "csp/SocketIO.hh"

#include <fmt/format.h>

#include <cstdint>

#include "csp/CSPPlatImports.hh"
#include "csp/csp.hh"

// TODO: Look into logging WSAGetLastError and strerror(errno)
// Using the logging object in csp.hh?
int SocketIO::send(socket_t sckt, const char *buf, int size, int flags) {
  uint32_t bytesSent;
  if ((bytesSent = ::send(sckt, (char *)buf, size, 0)) == err_code) {
    throw Ex2(
        Errcode::SOCKET_SEND,
        fmt::format("send() failed with error code %d", GETSOCKETERRNO()));
  }
  return bytesSent;
}

int SocketIO::recv(socket_t sckt, const char *buf, int size, int flags) {
  uint32_t bytesRecv;
  if ((bytesRecv = ::recv(sckt, (char *)buf, size, 0)) == err_code) {
    throw Ex2(
        Errcode::SOCKET_SEND,
        fmt::format("recv() failed with error code %d", GETSOCKETERRNO()));
  }
#ifdef _WIN32
  if (bytesRecv == WSACONNRESET) {
    return -1;
  }
#elif __linux__
  if (bytesRecv == 0) {
    return -1;
  }
#endif
  return bytesRecv;
}