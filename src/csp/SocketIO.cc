#include "csp/SocketIO.hh"

#include <cstdint>

int SocketIO::send(socket_t sckt, const char *buf, int size, int flags){
  return ::send(sckt, (char *)buf, size, 0);
}

int SocketIO::recv(socket_t sckt, const char *buf, int size, int flags){
  return ::recv(sckt, (char *)buf, size, 0);
}