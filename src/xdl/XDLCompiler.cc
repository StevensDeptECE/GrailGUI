#include "xdl/XDLCompiler.hh"

#include "util/datatype.hh"

using namespace std;

void XDLCompiler::readfile() {
  char buf[4096];
  ifstream fin(filename.c_str());
  while (fin.getline(buf, sizeof(buf))) {
    lineNumber++;
    try {
    } catch (const Ex& e) {
      cout << lineNumber << ": " << e << '\n';
    } /*
   try{

   }
   catch(const Ex& e){
     switch(){  // if use Errcode
     case :
     }
     // cout << lineNumber << ": " << e << '\n';
   }
   */
  }
  fin.close();
}

XDLCompiler::XDLCompiler(const char filename[])
    : symbols(new SymbolTable(this)),
      errorCount(0),
      warningCount(0),
      filename(filename),
      lineNumber(0) {
  readfile();
}

XDLCompiler::XDLCompiler()
    : symbols(new SymbolTable(this)),
      errorCount(0),
      warningCount(0),
      filename(""),
      lineNumber(0) {}