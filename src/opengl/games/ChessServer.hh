#include <winsock2.h>
// dont move

#include <windows.h>
#include <ws2tcpip.h>

#include <iostream>

using namespace std;
class ChessServer {
 private:
  SOCKET serverSocket;
  LPCWSTR ip;

 public:
  bool accepting;
  bool acceptit = true;
  bool sent = false;
  char sendBuffer[200] = {0};
  char recvBuffer[200] = {0};
  char lastRecv[200] = {0};
  void initServer(LPCWSTR ip, bool accepting);
  ChessServer(LPCWSTR ip, bool accepting) : ip(ip), accepting(accepting) {
    initServer(ip, accepting);
  }
  void recvit(char message[], int size);
  void cleanup() { WSACleanup(); }
  ~ChessServer() { cleanup(); }
  void sendit(char message[], int size);
};

// when turn changes call recv
// send right before calling recv
// do i need a while loop?