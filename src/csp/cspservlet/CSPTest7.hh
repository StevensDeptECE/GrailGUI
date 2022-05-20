//
// Created by dkruger on 11/01/17.
//
#pragma once

#include <csp/CSPServlet.hh>
//#include "csp/HttpServlet.hh"
//#include "csp/HTTPRequest.hh"
#include "csp/cspservlet/Student.hh"
#include "util/List1.hh"
class CSPTest7 : public CSPServlet {
 public:
  CSPTest7() {}
  ~CSPTest7() {}
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
    cout << "Test7 is loaded\n";
    // Buffer& out = r.getOut();
    Student test1 = Student("John", "Smith", 12345);
    Student test2 = Student("Bobby", "Tables", 54321);
    cout << "test1: " << test1.getFirst() << "\n";
    cout << "test2: " << test2.getFirst() << "\n";
    cout << "\n";
    List1<Student> mylist;
    mylist.add(test1);
    // cout<<"item1: "<<mylist.getData(0).getFirst()<<"\n";
    mylist.add(test2);
    cout << "item1: " << mylist.getData(0).getFirst() << "\n";
    cout << "item1: " << mylist.getData(0).getLast() << "\n";
    cout << "item1: " << (int)mylist.getData(0).getID() << "\n";
    cout << "\n";
    cout << "item2: " << mylist.getData(1).getFirst() << "\n";
    cout << "item2: " << mylist.getData(1).getLast() << "\n";
    cout << "item2: " << (int)mylist.getData(1).getID() << "\n";

    Buffer& out = r.getOut();
    out.writeList(mylist);
  }
};
