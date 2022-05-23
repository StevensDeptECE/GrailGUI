#include "repo/ssl/SSLSocket.h"

int main(int argc, char *argv[]) {
  const char *addr = argc < 2 ? "127.0.0.1" : argv[1];
  const char *portnum = argc < 3 ? "8000" : argv[2];
  SSLSocket s = SSLSocket::createClient(addr, portnum);
}
