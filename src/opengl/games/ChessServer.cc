#include <opengl/games/ChessServer.hh>

void ChessServer::sendit(char message[], int size) {
  int byteCount = send(serverSocket, message, size, 0);
  if (byteCount > 0) {
    cout << "you sent: " << message << endl;
  } else {
    WSACleanup();
  }
  acceptit = true;
  sent = false;
}
void ChessServer::recvit(char message[], int size) {
  int byteCount = recv(serverSocket, message, size, 0);
  if (byteCount > 0) {
    cout << "you got: " << message << endl;
  } else {
    WSACleanup();
  }
  acceptit = false;
  sent = true;
}

void ChessServer::initServer(LPCWSTR ip, bool accepting) {
  int port = 55555;
  WSADATA wsaData;
  int wsaerr;
  WORD wVR = MAKEWORD(2, 2);
  wsaerr = WSAStartup(wVR, &wsaData);
  if (wsaerr != 0) {
    cout << "ahh shit winsock dll not found" << endl;
    return;
  } else {
    cout << "HEY NICE JOB" << endl;
  }
  serverSocket = INVALID_SOCKET;
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket == INVALID_SOCKET) {
    cout << "ya socket fucked up" << endl;
    return;
  } else {
    cout << "socket good" << endl;
  }
  sockaddr_in service;
  service.sin_family = AF_INET;
  InetPtonW(AF_INET, ip, &service.sin_addr.s_addr);
  service.sin_port = htons(port);
  if (accepting) {
    if (bind(serverSocket, (SOCKADDR *)&service, sizeof(service)) ==
        SOCKET_ERROR) {
      cout << "bind failed" << WSAGetLastError() << endl;
      closesocket(serverSocket);
      WSACleanup();
      return;
    } else {
      cout << "bind is ok" << endl;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
      cout << "you aint listenin like dat" << endl;
    } else {
      cout << "im listenin like dat" << endl;
    }
    serverSocket = accept(serverSocket, NULL, NULL);
    if (serverSocket == INVALID_SOCKET) {
      cout << "accept failed" << endl;
      WSACleanup();
      return;
    }
    cout << "accepted connection" << endl;
  } else {
    if (connect(serverSocket, (SOCKADDR *)&service, sizeof(service)) ==
        SOCKET_ERROR) {
      cout << "connect failed" << WSAGetLastError() << endl;
      closesocket(serverSocket);
      WSACleanup();
      return;
    } else {
      cout << "connect is ok" << endl;
    }
  }
}