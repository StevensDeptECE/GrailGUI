#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600  // Need to be at least Vista
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib?
#pragma comment(lib, "Ws2_32.lib")  // Ignored by MinGW

#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) (closesocket(s))
#define GETSOCKETERRNO() (WSAGetLastError())
#else  // linux
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) (close(s))
#define GETSOCKETERRNO() (errno)
#define INVALID_SOCKET -1
#endif

#ifndef socket_t
using socket_t = decltype(socket(0, 0, 0));
#endif