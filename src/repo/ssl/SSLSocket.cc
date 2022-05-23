#include "SSLSocket.h"

#include <arpa/inet.h> /*for using ascii to network bit*/
#include <netdb.h>     // definitions for network database operations
#include <string.h>    /*using fgets funtions for geting input from user*/
#include <unistd.h> /*FOR USING FORK for at a time send and receive messages*/

#include "openssl/err.h" /* helps in finding out openssl errors*/
#include "openssl/ssl.h" /*using openssl function's and certificates and configuring them*/

bool SSLSocket::init = false;

SSLSocket::~SSLSocket() {
  if (ssl != nullptr) {
    SSL_free(ssl); /* release SSL state */
  }
  if (socket >= 0) {
    close(socket); /* close connection */
  }
  if (ctx != nullptr) {
    SSL_CTX_free(ctx); /* release context */
  }
}
SSLSocket::SSLSocket(const char *ip_addr, const char *port) {
  socket = ::socket(PF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr)); /*free output the garbage space in memory*/
  addr.sin_family = AF_INET;  /*getting ip address form machine */
  int portnum = atoi(port);
  addr.sin_port = htons(portnum); /* converting host bit to n/w bit */
  const int BUFFER = 1024;

  if (!init) {
    SSL_library_init();
    OpenSSL_add_all_algorithms(); /* load & register all cryptos, etc. */
    SSL_load_error_strings();     /* load all error messages */
    init = true;
  }

  if (ip_addr == nullptr) {
    addr.sin_addr.s_addr = INADDR_ANY;

  } else {
    struct hostent *host;

    if (!(host = gethostbyname(ip_addr))) {
      throw Ex(__FILE__, __LINE__, Errcode::SERVER_INVALID);
    }
    addr.sin_addr.s_addr = *(long *)(host->h_addr);
  }
}
SSLSocket::SSLSocket(SSLSocket &&orig)
    : ctx(orig.ctx), socket(orig.socket), ssl(orig.ssl) {
  orig.ssl = nullptr;
  orig.socket = -1;
  orig.ctx = nullptr;
}

SSLSocket SSLSocket::createServer(const char *port) {
  SSLSocket s(nullptr, port);

  if (bind(s.socket, (struct sockaddr *)&s.addr,
           sizeof(addr))) /* assiging the ip address and port*/ {
    throw Ex(__FILE__, __LINE__, Errcode::NO_BIND);
  }

  if (listen(s.socket,
             10)) /*for listening to max of 10 clients in the queue*/ {
    throw Ex(__FILE__, __LINE__, Errcode::LISTEN);
  }

  const SSL_METHOD *method =
      TLS_server_method();     /* create new server-method instance */
  s.ctx = SSL_CTX_new(method); /* create new context from method */

  if (s.ctx == nullptr) {
    // ERR_print_errors_fp(stderr);
    // Using an undefined error here because there weren't any in Errcode.hh
    // that fit
    throw Ex(__FILE__, __LINE__, Errcode::UNDEFINED);
  }

  socklen_t len = sizeof(addr);
  int client = accept(s.socket, (struct sockaddr *)&s.addr,
                      &len); /* accept connection as usual */
  printf("Connection: %s:%d\n", inet_ntoa(s.addr.sin_addr),
         ntohs(s.addr.sin_port)); /*printing connected client information*/

  s.ssl = SSL_new(s.ctx);    /* get new SSL state with context */
  SSL_set_fd(s.ssl, client); /* set connection socket to SSL state */
  char buf[1024];
  int sd, bytes;
  char input[1024];

  if (SSL_accept(s.ssl) == -1) /* do SSL-protocol accept */ {
    ERR_print_errors_fp(stderr);
  } else {
    // ShowCerts(s.ssl); /* get any certificates */
  }

  /*Fork system call is used to create a new process*/
  pid_t cpid;
  if (!(cpid = fork())) {
    while (1) {
      bytes =
          SSL_read(s.ssl, buf,
                   sizeof(buf)); /* get request and read message from server*/
      if (bytes > 0) {
        buf[bytes] = 0;
        printf("\nMESSAGE FROM SERVER:%s\n", buf);
      } else {
        ERR_print_errors_fp(stderr);
      }
    }
  } else {
    while (1) {
      printf("\nMESSAGE TO CLIENT:");
      fgets(input, 1024, stdin); /* get request and reply to client*/
      SSL_write(s.ssl, input, strlen(input));
    }
  }
  return s;
}

void SSLSocket::ShowCertificate() {
  // showing certs
  X509 *cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
  if (cert != nullptr) {
    printf("Server certificates:\n");
    char *line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    free(line); /* free the malloc'ed string */
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);      /* free the malloc'ed string */
    X509_free(cert); /* free the malloc'ed certificate copy */
  } else {
    printf("Info: No client certificates configured.\n");
  }
}

SSLSocket SSLSocket::createClient(const char *ip_addr, const char *port) {
  SSLSocket s(ip_addr, port);
  char buf[1024];
  const int BUFFER = 1024;
  char input[BUFFER];
  int bytes;
  pid_t cpid; /* fork variable*/

  const SSL_METHOD *method =
      TLS_client_method();     /* Create new client-method instance */
  s.ctx = SSL_CTX_new(method); /* Create new context */

  if (s.ctx == nullptr) {
    ERR_print_errors_fp(stderr);
    // Using an undefined error here because there weren't any in Errcode.hh
    // that fit
    throw Ex(__FILE__, __LINE__, Errcode::UNDEFINED);
  }

  if (connect(s.socket, (struct sockaddr *)&ip_addr,
              sizeof(ip_addr))) /*initiate a connection on a socket*/ {
    close(s.socket);
    throw Ex(__FILE__, __LINE__, Errcode::SERVER_INVALID);
  }

  s.ssl = SSL_new(s.ctx);      /* create new SSL connection state */
  SSL_set_fd(s.ssl, s.socket); /* attach the socket descriptor */

  if (SSL_connect(s.ssl) == -1) /* perform the connection */ {
    ERR_print_errors_fp(stderr);
  } else {
    printf("Connected with %s encryption\n", SSL_get_cipher(s.ssl));

    s.ShowCertificate();

    /* get any certs */
    /*Fork system call is used to create a new process*/
    if (!(cpid = fork())) {
      while (1) {
        printf("\nMESSAGE TO SERVER:");
        fgets(input, BUFFER, stdin);
        SSL_write(s.ssl, input, strlen(input)); /* encrypt & send message */
      }
    } else {
      while (1) {
        bytes = SSL_read(s.ssl, buf, sizeof(buf)); /* get request */
        if (bytes > 0) {
          buf[bytes] = 0;
          printf("\nMESSAGE FROM CLIENT: %s\n", buf);
        }
      }
    }
    SSL_free(s.ssl); /* release connection state */
  }
  close(s.socket);     /* close socket */
  SSL_CTX_free(s.ctx); /* release context */

  return s;
}
