//
// Created by dkruger on 11/01/17.
//
#pragma once

#include <csp/CSPServlet.hh>
//#include "csp/HttpServlet.hh"
//#include "csp/HTTPRequest.hh"
#include "util/List1.hh"
#include "csp/cspservlet/Student.hh"
class CSPTest6 : public CSPServlet {
private:

public:
    CSPTest6() {}
    ~CSPTest6() {}
    void request(CSPRequest& r) {
		/*
		 Buffer& out = r.getOut();
		List1<uint32_t> mylist;
		for (int i = 0; i < 15; i++){
			mylist.add(i);
		}
		//out << mylist;
		out.writeList(mylist);
		 */
        /*
		Buffer& out = r.getOut();
		List1<Student> mylist;
		mylist.add(Student("Ivan", "Valiaev", 12345))
		out << mylist;
         */
		cout<<"Test6 is loaded\n";
		Buffer& out = r.getOut();
		Student test = Student("Ivan", "Valiaev",12345);
		out.writeStudent(test);
    }
};
