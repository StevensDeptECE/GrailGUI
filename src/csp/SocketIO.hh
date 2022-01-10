#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdlib>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

// Compiler is not a fan of this define
//#define errno WSAGetLastError()
#else  // linux

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#endif

using socket_t = decltype(socket(0, 0, 0));

namespace SocketIO {
namespace {
#ifdef __linux__
constexpr int err_code = -1;
#elif _WIN32
constexpr int err_code = SOCKET_ERROR;
#endif
}  // anonymous namespace for errors

ssize_t send(socket_t sckt, const char *buf, int size, int flags);
ssize_t recv(socket_t sckt, char *buf, int size, int flags);
};  // namespace SocketIO