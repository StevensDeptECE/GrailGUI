/*
 * HttpRequest.cpp
 *
 *  Created on: Jun 21, 2014
 *      Author: AndresRicardo
 *  Streamlined by Dov Kruger.  HTTPRequest now handles the result from a
 *  Socket object for HTTP protocol.
 *  Sockets are completely encapsulated there.
 *  The Request object merely processes the bytes that came in.
 */

#include "csp/HTTPRequest.hh"

#include <servlets/Test1.h>
#include <servlets/Test2.h>
#include <servlets/Test3.h>
#include <servlets/Test5.h>
#include <servlets/Test6.h>
#include <servlets/Test7.h>
#include <servlets/benchmark0.h>
#include <servlets/benchmark1.h>
#include <servlets/benchmark2.h>
#include <servlets/benchmark3.h>
#include <servlets/benchmark4.h>
#include <servlets/benchmark5.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "csp/HttpServlet.hh"

using namespace std;

const string HTTPRequest::POST = "POST";
const string HTTPRequest::GET = "GET";
const string HTTPRequest::UNIMPLEMENTED = "???";

HTTPRequest::HTTPRequest(const char* ReqType) : Request(), servlets(1024) {
  if (strcmp(ReqType, "SERVER") == 0) {
    servlets.add("test1.hsp", new Test1);
    servlets.add("test2.hsp", new Test2);
    servlets.add("test3.hsp", new Test3);
    servlets.add("test5.hsp", new Test5);
    servlets.add("test6.hsp", new Test6);
    servlets.add("test7.hsp", new Test7);
    servlets.add("benchmark0.hsp", new Benchmark0);
    servlets.add("benchmark1.hsp", new Benchmark1);
    servlets.add("benchmark2.hsp", new Benchmark2);
    servlets.add("benchmark3.hsp", new Benchmark3);
    servlets.add("benchmark4.hsp", new Benchmark4);
    servlets.add("benchmark5.hsp", new Benchmark5);
  } else if (strcmp(ReqType, "CLIENT") == 0) {
    //        servlets = NULL;
  }
}

HTTPRequest::~HTTPRequest() {
  if (url != nullptr) delete url;
  if (host != nullptr) delete host;
  if (httpVersion != nullptr) delete httpVersion;
}
#if 0
inline const char * HTTPRequest::getNextToken(int &cursor, int &tokenLength) {
	while (isspace(buffer[cursor])) {
		cursor++;
	}
	int start = cursor;
	while (!isspace(buffer[cursor])) {
		cursor++;
		if (cursor >= size) {
			//TODO: max size currently hardcoded to 32768 bytes
			throw "Fix error: can't handle huge inputs";
			//			dataSize = recv(listenSock, buffer, size, 0);
			cursor = 0;
		}
	}
	tokenLength = cursor - start;
	return buffer+start;
}

void printString(const char *ptr) {
    for( ; *ptr!=NULL; ++ptr)
        printf("%c", *ptr);
}
#endif

// TODO: this code used to use char* buffer.  Now make it use Buffer object "in"
void HTTPRequest::tokenize() {
  //	Buffer& in = getIn(); // Buffer "in" is in HTTPRequest class
  //	int cursor = 0;
  //	const char * currentToken = getNextToken(cursor, currentTokenLength);

  uint32_t currentTokenLength = 0;

  if (in.parseToken(POST))
    method = POST.c_str();
  else if (in.parseToken(GET))
    method = GET.c_str();
  else
    method = UNIMPLEMENTED.c_str();

  cout << "Method: " << method << endl;

  if (!in.getUrl(url, urlLength)) {  // what is the url and where is it?
    cerr << "can't find url";
    url = "/";
    urlLength = 1;
  }

  if (!in.getHTTPVersion(httpVersion, currentTokenLength)) {
    cerr << "Can't read HTTP version number" << endl;
  }
  //
  if (!in.getHost(host, hostLength)) {
    cerr << "can't read host" << endl;
  }

  //    in.pointToStart();

  //	body = getNextToken(cursor, currentTokenLength);
  //	bodySize = sizeof(body);
  //	bodyLength = currentTokenLength;
}

// Server side
void HTTPRequest::handle(int sckt) {
  in.attachRead(sckt);
  out.attachWrite(sckt);

  tokenize();
  cout << "HTTP version: " << httpVersion << endl;
  cout << "host: " << host << '\n';
  //	cout << "hostLength: " << hostLength << endl;
  cout << "url: " << url << '\n';
  //	cout  << "urllength: " << urlLength << endl;
  HttpServlet* srv = servlets.get(url, strlen(url));
  srv->request(out);
  cout << '\n';
#if 0
	if (url[urlLength-1]=='/')e
		;//	temp+= "/index.html";
 	else temp+= string(url, urlLength);
#endif
  //	HttpServlet* s = servlets.get(url, urlLength);
  //
  //  if (s == nullptr) {
  //		cerr << "url not found: "<< url << std::endl;
  //		return;
  //	}
  //	s->request((*this).getOut());
  //    out.flush();
}

// Client side
void HTTPRequest::handle(int sckt, const char* command) {
  out.attachWrite(sckt);
  out.append(command);
  out.flush();

  in.attachRead(sckt);
  in.displayHTTPRaw();
}

#define SERVER_STRING "lwcsp\n"

char NOT_FOUND[] = "HTTP/1.1 404 NOT FOUND\r\n" SERVER_STRING
                   "Content-Type: text/html\r\n\r\n"
                   "<HTML><TITLE>Not Found</TITLE>\r\n"
                   "<BODY><P>The server could not fulfill\r\n"
                   "your request because the resource specified\r\n"
                   "is unavailable or nonexistent.\r\n</BODY></HTML>\r\n";

const char HEADER[] = "HTTP/1.1 200 OK\r\n" SERVER_STRING "Content-Type: ";

// const unsigned int HEADER_SIZE = sizeof(HEADER)-1;

void error_die(const char* sc) {
  perror(sc);
  exit(1);
}

void unimplemented(int client) {
  const int SIZE = 256;
  char buf[SIZE];
  snprintf(
      buf, SIZE,
      "HTTP/1.1 501 Method Not Implemented\r\n%sContent-Type: text/html\r\n\r\n<HTML><HEAD><TITLE>Method Not Implemented\r\n</TITLE>\
            </HEAD>\r\n<BODY><P>HTTP request method not supported.\r\n</BODY></HTML>\r\n",
      SERVER_STRING);
  //	send(client, buf, strlen(buf), 0);
}
