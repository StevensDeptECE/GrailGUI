#include "data/GapMinderExtra.hh"
#include <fstream>
#include <sstream>
#include "data/BlockLoader2.hh"
#include <string>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <cmath>

using namespace std;

inline const char* align( const char* p) {
  return (const char*)(((uint64_t)p + 7) & ~uint64_t(7));
}


vector <int> getContinents(){
  ifstream r("countryCodes.txt");
  ifstream c("countryContinents.txt");

  vector <int> x;


  char buffer[4096];
  char buffer2[4096];
  char a[8];
  char b[8];
  while(r.getline(buffer, sizeof(buffer))){

    bool next = true;
    while(next){
      next = true;
      c.getline(buffer, sizeof(buffer2));
      istringstream line(buffer2);
      line.getline(a, sizeof(a));
      line.getline(b, sizeof(b));

      if(a == buffer){
        if(b == "NA"){
          x.push_back(0);
        }
        if(b == "EU"){
          x.push_back(1);
        }
        if(b == "AS"){
          x.push_back(2);
        }
        if(b == "SA"){
          x.push_back(3);
        }
        if(b == "AF"){
          x.push_back(4);
        }
        if(b == "OC"){
          x.push_back(5);
        }
        next = false;
      }

    }
  }

  return x;
}

  










