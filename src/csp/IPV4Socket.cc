#include "IPV4Socket.hh"

/*
  All encapsulation for different operating systems networking code is done here
*/

//#include "csp/HTTPRequest.hh"
#include <fmt/format.h>

#include <algorithm>

#include "csp/CSPPlatImports.hh"
#include "csp/SocketIO.hh"
#include "csp/csp.hh"

#ifdef _WIN32
WSADATA Socket::wsaData;
#elif __linux__
#include <unistd.h>

#include <cstdlib>
#include <cstring>
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

struct client_info {
  socklen_t addr_len = sizeof(addr);
  struct sockaddr_storage addr;
  socket_t socket;
  char request[MAX_REQUEST_SIZE + 1];
  int received;
};
fd_set waitOnClients(IPV4Socket *sckt, socket_t server);

IPV4Socket::~IPV4Socket() {
  for (auto &client : clients) {
    dropClient(&client);
  }

  CLOSESOCKET(sckt);
}

// Constructor for server
IPV4Socket::IPV4Socket(uint16_t port, const char *host) : Socket(port) {
  auto bind_addr = this->result;
  struct addrinfo hints;

  char err_buf[256];
  char port_string[8];
  sprintf(port_string, "%d", port);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the local address and port to be used by the server
  if (getaddrinfo(host, port_string, &hints, &bind_addr)) {
    freeaddrinfo(bind_addr);
    throw Ex2(Errcode::SOCKET, "Address already in use");
  }
  this->result = bind_addr;

  sckt = INVALID_SOCKET;

  // Create a SOCKET for the server to listen for client connections
  sckt = socket(bind_addr->ai_family, bind_addr->ai_socktype,
                bind_addr->ai_protocol);

  if (!ISVALIDSOCKET(sckt)) {
    freeaddrinfo(result);
    sprintf(err_buf, "socket() failed. %d", GETSOCKETERRNO());
    throw Ex2(Errcode::SOCKET, err_buf);
  }

  // Setup the TCP listening socket
  if (bind(sckt, result->ai_addr, (int)result->ai_addrlen)) {
    freeaddrinfo(result);
    sprintf(err_buf, "bind() failed. %d", GETSOCKETERRNO());
    throw Ex2(Errcode::SOCKET_BIND, err_buf);
  }

  freeaddrinfo(result);

  if (listen(sckt, SOMAXCONN) < 0) {
    sprintf(err_buf, "listen() failed. %d", GETSOCKETERRNO());
    throw Ex2(Errcode::LISTEN, err_buf);
  }

  // TODO: Check if send/receive needs to be implemented
  // TODO: Check how disconnect/shutdown should be handled
}

// Constructor for csp/http client
IPV4Socket::IPV4Socket(const char *addr, uint16_t port) : Socket(addr, port) {
  auto server_addr = this->result;
  struct addrinfo hints;

  char err_buf[100];
  char port_string[8];
  sprintf(port_string, "%d", port);
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  // Resolve the address and port to be used by the client
  if (getaddrinfo(addr, port_string, &hints, &server_addr)) {
    freeaddrinfo(server_addr);
    sprintf(err_buf, "getaddrinfo() failed. \nunable to resolve address %s:%d",
            addr, port);
    throw Ex2(Errcode::SOCKET, err_buf);
  }

  char address_buffer[100];
  char service_buffer[100];

  if (getnameinfo(server_addr->ai_addr, server_addr->ai_addrlen, address_buffer,
                  sizeof(address_buffer), service_buffer,
                  sizeof(service_buffer), NI_NUMERICHOST)) {
    freeaddrinfo(server_addr);
    throw Ex2(Errcode::SOCKET, "getnameinfo() failed");
  }
  sckt = socket(server_addr->ai_family, server_addr->ai_socktype,
                server_addr->ai_protocol);
  if (!ISVALIDSOCKET(sckt)) {
    freeaddrinfo(server_addr);
    throw Ex2(Errcode::SOCKET, "socket() failed");
  }

  if (connect(sckt, server_addr->ai_addr, server_addr->ai_addrlen)) {
    freeaddrinfo(server_addr);
    throw Ex2(Errcode::SOCKET, "connect() failed");
  }

  freeaddrinfo(server_addr);
}

// Server side
void IPV4Socket::wait() {
  char err_buf[100];

  while (true) {
    cout << "WAITING CONNECTION." << endl;
    fd_set reads;
    reads = waitOnClients(this, sckt);

    if (FD_ISSET(sckt, &reads)) {
      cout << "Connection successful. New Client connected." << endl;
      auto client = &clients.emplace_back();
      client->socket =
          accept(sckt, (struct sockaddr *)&client->addr, &client->addr_len);

      if (!ISVALIDSOCKET(client->socket)) {
        sprintf(err_buf, "accept() failed. (%d)\n", GETSOCKETERRNO());
        cerr << err_buf << endl;
        cerr << "Connection to client dropped." << endl;
        continue;
      }

      cout << "New connection from " << getClientAddress(client);
      handleClientConnection(client);
      continue;
    }
    for (auto &client : clients) {
      if (FD_ISSET(client.socket, &reads)) {
        cout << "Handling client connection from " << getClientAddress(&client)
             << ".\n";
        handleClientConnection(&client);
      }
    }
  }
}

void IPV4Socket::handleClientConnection(client_info *info) {
  auto status = req->handle(info->socket, true);
  if (req->getShouldClose()) {
    cout << fmt::format("Request for client %s closed.\n",
                        getClientAddress(info));
    dropClient(info);
  }
  if (!status.isWriting && status.bytesTransferred < 0) {
    cout << fmt::format("Unexpected disconnect from client %s.\n",
                        getClientAddress(info));
    dropClient(info);
  }
}

fd_set waitOnClients(IPV4Socket *sckt, socket_t server) {
  fd_set reads;
  FD_ZERO(&reads);

  FD_SET(server, &reads);
  socket_t max_socket = server;

  for (const auto &client : sckt->clients) {
    FD_SET(client.socket, &reads);
    if (client.socket > max_socket) {
      max_socket = client.socket;
    }
  }

  if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
    char err_buf[100];
    sprintf(err_buf, "select() failed. (%d)\n", GETSOCKETERRNO());
    throw Ex2(Errcode::SERVER_INVALID, err_buf);
  }

  return reads;
}

// Client side
void IPV4Socket::send(const char *command) { req->handle(sckt, command); }

void IPV4Socket::send(uint32_t reqn) {
  out.attachWrite(sckt);
  out.write(reqn);
  out.flush();
  in.attachRead(sckt);
}

void IPV4Socket::sendAndAwait(uint32_t reqn, Request &r) {
  send(reqn);
  r.handle(sckt, false);
}

auto getClient(IPV4Socket *sckt, socket_t sock) {
  auto isSock = [sock](client_info &c) { return c.socket == sock; };
  auto fr = ranges::find_if(sckt->clients, isSock);
  auto end = sckt->clients.end();
  if (fr != end) {
    return &*fr;
  }

  sckt->clients.emplace_back();
  return &(sckt->clients.back());
}

void IPV4Socket::dropClient(struct client_info *info) {
  CLOSESOCKET(info->socket);

  auto isSock = [info](client_info &c) { return c.socket == info->socket; };

  auto numErased = std::erase_if(clients, isSock);
  if (numErased == 0) {
    printf("Client with address %s not found\n",
           getClientAddress(info).c_str());
  }
}

string IPV4Socket::getClientAddress(struct client_info *info) {
  std::string address;
  address.reserve(100);
  char *address_buffer = address.data();
  getnameinfo((struct sockaddr *)&info->addr, info->addr_len, address_buffer,
              sizeof(address_buffer), 0, 0, NI_NUMERICHOST);

  return address;
}
