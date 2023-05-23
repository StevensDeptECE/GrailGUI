//
// Created by maquiles on 11/13/17.
//

#pragma once

#include <csp/csp.hh>
#include <iostream>

#include "csp/CSPRequest.hh"
#include "csp/CSPServlet.hh"
#include "csp/IPV4Socket.hh"
#include "csp/Page.hh"
#include "csp/PageBuffer.hh"
#include "csp/Request.hh"
#include "csp/Socket.hh"
#include "cspservlet/UtilityStructs.hh"
#include "util/Buffer.hh"
// === Removed for compatibility, use plibsys ===
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// ==============================================
#include <cstdio>

class CSPClient {
 private:
  //    int fd;
  IPV4Socket s;
  vector<Page> pages;
  void addDynamic(const DataType& type, char*& a);

 protected:
  Buffer& in;
  Buffer& out;

 public:
  CSPClient(char* addr, int port);
  pageBuffer pageRequest(uint32_t pageid);
  void testPrint(int pageid);

  void addPage(string staticData, vector<Insertion>& dynamic);
  void addPage(string staticData);
  void addPage(string staticFile, string offsetFile);
  void printPageData(int pageid) { cout << pages[pageid]; }
  pageBuffer pageRequestFile(uint32_t pageid);  // Requests page from text file
};