#include "csp/HttpServlet.hh"

class TestHttpServlet : public HttpServlet {
 public:
  void request(HTTPRequest& req) {
    std::cout << "Content-type: text/html\n\n";
    std::cout << "Hello";
  }
};
