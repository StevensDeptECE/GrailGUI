#pragma once

#include "xdl/SymbolTable.hh"

class XDLCompiler {
private:
  SymbolTable symbols;
  uint32_t errorCount;
  uint32_t warningCount;
  string filename;
  uint32_t lineNumber;
  void displayMessage(const std::string& msg) {
    //TODO use log? switch from string to integer message number
    cerr << filename << ": " << lineNumber << '\t' << msg << '\n';
  }
  void readfile();

public:
  XDLCompiler(const char filename[]);

  SymbolTable& getSymbolTable() { return symbols; }
  void generateCode() {
    if (errorCount > 0)
      return;
    // do whatever you do in this compiler
  }
  
  void error(const std::string& msg) {
    displayMessage(msg);
    errorCount++;
  }

  void duplicateSymbol(const std::string& name) {
    error(string("Duplicate Symbol ") + name);
  }

  void undefinedSymbol(const std::string& name) {
    error(string("Undefined Symbol ") + name);
  }

  void internalError(const std::string& name) {
    error(string("InternalError") + name);
  }

  void warning(const std::string& msg) {
    displayMessage(msg);
    warningCount++;
  }
};
