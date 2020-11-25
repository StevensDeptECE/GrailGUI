#include "IPV4Socket.hh"

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __linux__
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <memory.h>

#include "csp/HTTPRequest.hh"
#include "csp/csp.hh"

using namespace std;

void testResult(int result, const char *file, int lineNum, Errcode err) {
  if (result < 0) {
    throw Ex(file, lineNum, err);
  }
}


// Constructor for HTTP server
IPV4Socket::IPV4Socket(uint16_t port) : Socket(port) {
  int yes = 1;
  testResult(sckt = socket(AF_INET, SOCK_STREAM, 0), __FILE__, __LINE__,
             Errcode::SOCKET);
  testResult(setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, (const char *) &yes, sizeof(yes)),
             __FILE__, __LINE__, Errcode::SETSOCKOPT);
  memset((char *)&sockaddress, 0, sizeof(sockaddress));
  sockaddress.sin_family = AF_INET;
  sockaddress.sin_addr.s_addr = INADDR_ANY;
  sockaddress.sin_port = htons(port);
  ::bind(sckt, (struct sockaddr *)&sockaddress, sizeof(sockaddress));
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

  sockaddress.sin_family = AF_INET;
  //    bcopy((char *)server->h_addr, (char *)&sockaddress.sin_addr.s_addr,
  //    server->h_length);
  sockaddress.sin_addr.s_addr = inet_addr(address);
  sockaddress.sin_port = htons(port);

  if (connect(sckt, (struct sockaddr *)&sockaddress, sizeof(sockaddress)) < 0) {
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
    //		int senderSock = accept(listenSock, (struct sockaddr *) &sockaddress,
    //&senderNameLen);
    if (returnsckt >= 0) {
      cout << "CONNECT SUCCESSFULLY"
           << "\n";
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

// Client side
void IPV4Socket::send(const char *command) { req->handle(sckt, command); }

void IPV4Socket::send(uint32_t reqn) {
  out.attachWrite(sckt);
  out.write(reqn);
  out.flush();
  in.attachRead(sckt);
}
