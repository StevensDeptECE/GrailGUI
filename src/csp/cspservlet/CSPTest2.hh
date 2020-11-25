//
// Created by dkruger on 11/01/17.
//
#pragma once

#include <csp/CSPServlet.hh>

class CSPTest2 : public CSPServlet {
private:

public:
    CSPTest2() {}
    ~CSPTest2() {}
    void request(CSPRequest& r) {
        cout<<"Test2 is loaded\n";
        Buffer& out = r.getOut();
        for (uint32_t x = 0; x < 1024; x++)
            //out.write(x);
            out<<x;
        }
};
