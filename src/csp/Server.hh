//
// Created by dkruger on 5/16/18.
//
#pragma once
#include "csp/csp.hh"
#include "csp/IPV4Socket.hh"
#include "csp/HTTPRequest.hh"

template<typename SocketImpl, typename HandlerImpl>
class Server {
private:
    SocketImpl s;
    HandlerImpl req;
public:
    Server(int port) : s(port) {
        s.attach(req);
        try {
            s.wait();
        } catch (const Ex& e) {
            cerr << e << '\n';
        }
    }
};
