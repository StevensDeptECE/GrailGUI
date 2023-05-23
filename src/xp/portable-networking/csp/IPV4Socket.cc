#include "IPV4Socket.hh"

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
/*
  All encapsulation for different operating systems networking code is done here
*/

#include <memory.h>
#include <signal.h>

//#include "csp/HTTPRequest.hh"
#include "csp/SocketIO.hh"
#include "csp/csp.hh"

#ifdef _WIN32
WSADATA Socket::wsaData;
#endif

using namespace std;

#ifdef __linux__
inline void testResult(int result, const char *file, int lineNum, Errcode err) {
  if (result < 0) {
    throw Ex(file, lineNum, err);
  }
}

#elif _WIN32
inline void testResult(int result, const char *file, int lineNum, Errcode err) {
  if (result != 0) {
    throw Ex(file, lineNum, err);
  }
}
#endif

// Initializes Winsock
void Socket::classInit() {
#ifdef _WIN32
  testResult(WSAStartup(MAKEWORD(2, 2), &wsaData), __FILE__, __LINE__,
             Errcode::SOCKET);
#endif
  return;
}

// Takes care of allocations made by Winsock
void Socket::classCleanup() {
#ifdef _WIN32
  WSACleanup();
#endif
}

#ifdef __linux__
// Constructor for HTTP server
IPV4Socket::IPV4Socket(uint16_t port) : Socket(port) {
  int yes = 1;
  testResult(sckt = socket(AF_INET, SOCK_STREAM, 0), __FILE__, __LINE__,
             Errcode::SOCKET);
  testResult(setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes,
                        sizeof(yes)),
             __FILE__, __LINE__, Errcode::SETSOCKOPT);
  memset(sockaddress, 0, sizeof(sockaddress));
  sockaddr_in *sockAddr = (sockaddr_in *)sockaddress;
  sockAddr->sin_family = AF_INET;
  sockAddr->sin_addr.s_addr = INADDR_ANY;
  sockAddr->sin_port = htons(port); // 1025 = 00000000 00000000 00000100 00000001 0x00000401
  ::bind(sckt, (struct sockaddr *)sockAddr, sizeof(sockaddr_in));
  testResult(listen(sckt, 20), __FILE__, __LINE__, Errcode::LISTEN);
}

// Constructor for csp/http client
IPV4Socket::IPV4Socket(const char *addr, uint16_t port) : Socket(addr, port) {
  struct hostent *server;
  testResult(sckt = socket(AF_INET, SOCK_STREAM, 0), __FILE__, __LINE__,
             Errcode::SOCKET);
  server = gethostbyname(address);

  if (server == nullptr) {
    throw Ex(__FILE__, __LINE__, Errcode::SERVER_INVALID);
  }

  sockaddr_in *sockAddr = (sockaddr_in *)sockaddress;
  sockAddr->sin_family = AF_INET;
  //    bcopy((char *)server->h_addr, (char *)&sockaddress.sin_addr.s_addr,
  //    server->h_length);
  sockAddr->sin_addr.s_addr = inet_addr(address);
  sockAddr->sin_port = htons(port);

  if (connect(sckt, (struct sockaddr *)sockaddress, sizeof(sockaddr_in)) < 0) {
    throw Ex(__FILE__, __LINE__, Errcode::CONNECTION_FAILURE);
  }
}

// Server side
void IPV4Socket::wait() {
  struct sockaddr_in client_addrconfig;
  socklen_t client_length = sizeof(client_addrconfig);

  while (true) {
    cout << "WAITING CONNECTION." << endl;
    int returnsckt =
        accept(sckt, (struct sockaddr *)&client_addrconfig, &client_length);
    //		int senderSock = accept(listenSock, (struct sockaddr *)
    //&sockaddress, &senderNameLen);
    if (returnsckt >= 0) {
      cout << "CONNECT SUCCESSFULLY"
           << "\n";
      req->handle(returnsckt);
      close(returnsckt);
      // if you are not familiar with socket, try below code
      //			read(senderSock,testin, sizeof(testin)-1);
      //			cout<<testin<<endl;
      //			strcpy(testout,"hello,this is server");
      //			write(senderSock,testout,sizeof(testout));
      //			cout<<listenSock;
    } else {
      throw Ex(__FILE__, __LINE__, Errcode::CONNECTION_FAILURE);
    }
  }
}
#endif

#ifdef _WIN32

// Constructor for server
IPV4Socket::IPV4Socket(uint16_t port) : Socket(port) {
  struct addrinfo *result = NULL;
  struct addrinfo *ptr = NULL;
  struct addrinfo hints;

  char port_string[8];
  itoa(port, port_string, 10);

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = 1;  // TODO: Originally AI_PASSIVE, convert to macro

  // TODO: Find out if port should stay as uint16_t or convert to const char *
  // Resolve the local address and port to be used by the server
  testResult(getaddrinfo(NULL, port_string, &hints, &result), __FILE__,
             __LINE__, Errcode::SOCKET);

  sckt = INVALID_SOCKET;

  // Create a SOCKET for the server to listen for client connections
  sckt = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

  // TODO: Determine how to best convert for use with test result
  if (sckt == INVALID_SOCKET) {
    // WSACleanup();
    freeaddrinfo(result);
    throw Ex1(Errcode::SOCKET);
  }

  // Setup the TCP listening socket
  if (bind(sckt, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
    cerr << WSAGetLastError() << endl;
    freeaddrinfo(result);
    closesocket(sckt);
    throw Ex1(Errcode::SOCKET_BIND);
    // WSACleanup();
  }

  freeaddrinfo(result);

  if (listen(sckt, SOMAXCONN) == SOCKET_ERROR) {
    cerr << "listen failed with error: " << WSAGetLastError() << endl;
    closesocket(sckt);
    // WSACleanup();
    return;
  }

  // TODO: Check if send/receive needs to be implemented
  // TODO: Check how disconnect/shutdown should be handled
}

// Constructor for csp/http client
IPV4Socket::IPV4Socket(const char *addr, uint16_t port) : Socket(addr, port) {
  sckt = INVALID_SOCKET;
  struct addrinfo *result = NULL;
  struct addrinfo *ptr = NULL;
  struct addrinfo hints;

  char port_string[8];
  itoa(port, port_string, 10);
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // TODO: Find out if port should stay as uint16_t or convert to const char *
  // Resolve the address and port to be used by the client
  testResult(getaddrinfo(addr, port_string, &hints, &result), __FILE__,
             __LINE__, Errcode::SOCKET);

  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
    // Create a SOCKET for connecting to server
    sckt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    testResult(sckt == INVALID_SOCKET, __FILE__, __LINE__, Errcode::SOCKET);

    // TODO: Check if connect should be passed to test_result and
    // closesocket
    //       reimplemented in destructor
    if (connect(sckt, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
      closesocket(sckt);
      sckt = INVALID_SOCKET;
      continue;
    }
    break;
  }

  testResult(sckt == INVALID_SOCKET, __FILE__, __LINE__, Errcode::SOCKET);

  // TODO: Get some more info on this
  // Probably should remove and leave to the destructor???
  freeaddrinfo(result);
}

// Server side
void IPV4Socket::wait() {
  struct sockaddr_in client_addrconfig;
  socklen_t client_length = sizeof(client_addrconfig);
  SOCKET returnsckt = INVALID_SOCKET;

  while (true) {
    cout << "WAITING CONNECTION." << endl;

    // Accept a client socket
    returnsckt = accept(sckt, NULL, NULL);
    if (returnsckt == INVALID_SOCKET) {
      cerr << "accept failed with error: " << WSAGetLastError() << endl;
      closesocket(sckt);
      // WSACleanup();
      return;
    }

    if (returnsckt >= 0) {
      cout << "CONNECT SUCCESSFULLY" << endl;
      req->handle(returnsckt);
      close(returnsckt);
      // csp18summer: if you are not familiar with socket, try below code
      //			read(senderSock,testin, sizeof(testin)-1);
      //			cout<<testin<<endl;
      //			strcpy(testout,"hello,this is server");
      //			write(senderSock,testout,sizeof(testout));
      //			cout<<listenSock;
    } else {
      throw Ex(__FILE__, __LINE__, Errcode::CONNECTION_FAILURE);
    }
  }
}

#endif

// Client side
void IPV4Socket::send(const char *command) { req->handle(sckt, command); }

void IPV4Socket::send(uint32_t reqn) {
  out.attachWrite(sckt);
  out.write(reqn);
  out.flush();
  in.attachRead(sckt);
}
