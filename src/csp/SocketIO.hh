#pragma once

#ifdef _WIN32
// socket_t should be SOCKET, but that requires importing winsock
#define socket_t unsigned long long
#else
#define socket_t int
#endif

namespace SocketIO {
#ifdef _WIN32
#define err_code SOCKET_ERROR
#else
#define err_code -1
#endif
int send(socket_t sckt, const char *buf, int size, int flags);
int recv(socket_t sckt, const char *buf, int size, int flags);
};  // namespace SocketIO