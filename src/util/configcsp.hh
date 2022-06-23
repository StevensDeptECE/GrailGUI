// Config file for CSP

#ifndef CONFIGCSP_HH_
#define CONFIGCSP_HH_

#include <string.h>

#include <fstream>
#include <iostream>

#include "Config.hh"

/**
NEEDED: We need a hardcoded list of what all is needed in the proj

check_correct(Hashmap map1<k,v>):
        for name in hardcoded_list:
                if typeOF(map1[name]) == type we need/within enum/range
                        continue
                else
                        raise error with type_name
                        print error to log
                        exit 1

*/

enum prot = {"csp", "https", "http"};  // only protocols we'll use
enum ip = {"ipV4", "ipV6"};  // only IP versions we'll use at the moment

class csp {
 public:
  uint16_t port;     // Port number to talk/listen on
  prot protocol;     // Specifies what protocol we're going to use
  ip ip_ver;         // Version if IP we're going to use
  std::string log;   // The name of the log file to be generated
  std::string base;  // this depends on os, base for all directory
  buf buffer;

  csp() {  // TODO: Write the tokenizer and make the fix the x[1] thing, because
           // the
    std::ifstream reader;
    char x[10];
    std::string line;
    reader.open("csp.conf", std::ios::in);
    while (!reader.eof()) {
      reader >> x;
      // split x into words
      if (strcmp(x, "port"))
        port = uint16_t(x[1]);
      else if (strcmp(x, "log"))
        log = x[1];
      else if (strcmp(x, "protocol"))
        protocol = x[1];
    }
  }
};

#endif