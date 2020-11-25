//
// Created by dkruger on 11/01/17.
//
#pragma once

#include <csp/CSPServlet.hh>
//#include "csp/HttpServlet.hh"
//#include "csp/HTTPRequest.hh"
class CSPTest3 : public CSPServlet {
private:

public:
    CSPTest3() {}
    ~CSPTest3() {}
    void request(CSPRequest& r) {
        cout<<"Test3 is loaded\n";
        Buffer& out = r.getOut();
        string first = "Dov";
        string last = "Kruger";
        out << first << last;
    }
};
