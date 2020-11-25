//
// Created by dkruger on 11/01/17.
//
#pragma once

#include <csp/CSPServlet.hh>
//#include "csp/HttpServlet.hh"
//#include "csp/HTTPRequest.hh"
#include "util/List1.hh"
#include "csp/CSPRequest.hh"

class CSPTest5 : public CSPServlet {
private:

public:
    CSPTest5() {}
    ~CSPTest5() {}
    void request(CSPRequest& r) {
        Buffer& out = r.getOut();
		List1<uint32_t> mylist;
		for (int i = 0; i < 15; i++){
			mylist.add(i);
		}
		for (int i = 0; i < 15; i++){
			cout<<(int)mylist.getData(i)<<"\n";
		}
		//out << mylist;
		out.writeList(mylist);
    }
};
