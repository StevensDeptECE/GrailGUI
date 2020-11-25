#pragma once

#include "Socket.hh"
#include "csp/Request.hh"
#include <cstdint>
#include <string>

class UDPV4Socket : public Socket {
private:
	int sckt;

public:
  UDP4Socket(const char* addr, uint16_t port); //Client
	UDP4Socket(uint16_t port); // Server
  ~IPV4Socket(){
    close(sckt);
  }
	void listenOnPort();
	void wait();
	void send(const char* buf, uint32_t len);
  //  void ack(BadPackets p);
	void send(uint32_t reqn); // For CSP
};
