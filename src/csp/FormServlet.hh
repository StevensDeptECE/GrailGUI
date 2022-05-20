#include "csp/HttpServlet.hh"

class FormServlet : public HttpServlet {
 public:
  void request(Request& r) {
    // this is the way it would work in CSP
    //		string firstName = in.readStringCheck();
    // string lastName =  in.readStringCheck();
    // int age = in.readUInt32Check();
    string firstName = r.getParameter("firstName");
    out.append("Content-type: text/html\n\n");
    out.append("This is my servlet");
    out.append(2 + 3);  // --> append '5'

#if 0
		int x[] = {1, 3, 5, 7, 22, 12, 14};
		out.write(x, 7);
#endif
  }
};
