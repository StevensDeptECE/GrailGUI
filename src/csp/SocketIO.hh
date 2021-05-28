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

#else // linux

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

using socket_t = decltype(socket(0, 0, 0));

class SocketIO {
  public:

    static int send(socket_t sckt, const char *buf, int size, int flags);
    static int recv(socket_t sckt, const char *buf, int size, int flags);
};