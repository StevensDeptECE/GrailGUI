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
  bool accepting;

 public:
  bool acceptit = true;
  bool sent = false;
  char sendBuffer[200];
  char recvBuffer[200];
  char lastRecv[200];
  ChessServer(LPCWSTR ip, bool accepting) : ip(ip), accepting(accepting){};
  void initServer(LPCWSTR ip, bool accepting);
  void sendit(char message[], int size);
  void recvit(char message[], int size);
  void cleanup() { WSACleanup(); }
};

// when turn changes call recv
// send right before calling recv
// do i need a while loop?