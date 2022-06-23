#pragma once
#include <arpa/inet.h> /*for using ascii to network bit*/

#include "opengl/Errcode.hh"
#include "openssl/ssl.h"
#include "util/Ex.hh"

class SSLSocket {
 private:
  SSL_CTX *ctx;
  int socket;
  SSL *ssl;
  struct sockaddr_in addr; /*creating the sockets*/
  static bool init;
  SSLSocket(const char *ip_addr, const char *port);
  void ShowCertificate();

 public:
  ~SSLSocket();
  SSLSocket(const SSLSocket &orig) = delete;
  SSLSocket(SSLSocket &&orig);

  static SSLSocket createServer(const char *port);
  static SSLSocket createClient(const char *addr, const char *port);
};
