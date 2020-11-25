#pragma once

#include "Socket.hh"
#include "csp/Request.hh"
#include <cstdint>
#include <string>

class IPV4Socket : public Socket {
private:
	int sckt;

public:
	IPV4Socket(const char* addr, uint16_t port); //Client
	IPV4Socket(uint16_t port); // Server
    ~IPV4Socket(){
        close(sckt);
    }
    //csp18summer: It may be used in future. for server
    //IPV4Socket(uint16_t port);
    //csp18summer: Not defined so far
	void listenOnPort();
	//	void send(const char data[], size_t bytes);
	//	size_t receive(char data[], size_t bytes);
	void wait();
	void send(const char* command); // For HTTP
	void send(uint32_t reqn); // For CSP
};
