#include "csp/HTTPSRequest.hh"

void HttpsRequest::read() {
  SSL_library_init();
  dataSize = SSL_read(ssl, data, BUFFER_SIZE);
  if (dataSize > 0) {
    data[dataSize] = 0;
    cout << "Client MSG:" << data << endl;
    url = data;
    urlLength = dataSize;
  } else {
    cout << "didn`t receive data" << endl;
    ERR_print_errors_fp(stderr);
  }
}

HttpsRequest::HttpsRequest(SSL* ssl, const int socketId)
    : HttpRequest(socketId), ssl(ssl) {
  if (SSL_accept(ssl) == -1) /* do SSL-protocol accept */
    ERR_print_errors_fp(stderr);
  else {
    read();
    handler();
  }
}

HttpsRequest::~HttpsRequest() { cout << "delete HttpsRequest" << endl; }
