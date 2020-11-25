//
// Created by yxie14 on 7/27/17.
//
#pragma once

#include <csp/CSPServlet.hh>
//#include "csp/HttpServlet.hh"
//#include "csp/HTTPRequest.hh"
class CSPTest1 : public CSPServlet {
private:

public:
    CSPTest1() {}
    ~CSPTest1() {}
    void request(CSPRequest& r) {
        cout<<"Test1 is loaded\n";
        Buffer& out = r.getOut();
        uint32_t x = 12345;
        out << x ;
        //out.flush(); // TODO: this should not be necessary
    }
};
