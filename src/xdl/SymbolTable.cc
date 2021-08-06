#include "xdl/SymbolTable.hh"

#include "xdl/XDLCompiler.hh"
#include "xdl/std.hh"

//#include "symTable.h"
#include <iostream>

#include "util/Buffer.hh"
using namespace std;

// GrailParser *parser;

SymbolTable::SymbolTable(XDLCompiler* c) : Struct(c), root(0) {
  // TODO: what does it mean to have root = 0 if there are no members? Not the
  // greatest design
}

void SymbolTable::addRoot(const XDLType* t) {
  root = members.size();  // root will point at the last object which is the
                          // root object in the table
  members.add(Member(
      0, 0,
      t));  // set the root object for this symbol table Usually a structure
}

Struct* SymbolTable::addStruct(const string& name) {
  Struct* s = new Struct(compiler,
                         name);  // TODO: this allocates the structure even if
                                 // it's already defined, which may not be great
  // The compiler should then delete?
  addSymCheckDup(name, s);
  // create a new struct and return so members may be added
  return s;
}

void SymbolTable::addXDLType(const std::string& name, XDLType* xdlType) {
  addSymCheckDup(name, xdlType);
}
#if 0
// let's try letting symbols table write itself exactly like a struct for now
void SymbolTable::writeMeta(Buffer& metadataBuf) {
  metadataBuf.write(uint8_t(members.size())); // first write how many types are in the symbol table, max=255
  for (uint32_t i = 0; i < members.size(); i++)
    types[i]->writeMeta(metadataBuf);
}
#endif

void SymbolTable::readMeta(Buffer& metadataBuf) {
  DataType dt = metadataBuf.readType();
  // Removed to allow Lists (and other containers) to act as top level
  // structures
  // if (dt != DataType::STRUCT8) {
  //   throw Ex1(Errcode::BAD_PROTOCOL);  // first byte must be STRUCT8
  // }

  string name = metadataBuf.readString8();  // the name of the symbol table,
                                            // probably 0 with no letters
  // uint8_t numElements = metadataBuf.readU8();
  XDLType::readMeta(compiler, metadataBuf);
  // TODO: this is PATHETIC! reading one extra byte. Let's figure out how to fix
  // the buge eventually...
  metadataBuf.readU8();  // munch one extra byte which is the empty string on
                         // the member name of the symbol table?
}

#if 0
// separate files for metadata and data
void SymbolTable::displayText(Buffer& binaryIn, Buffer& asciiOut) const{
  for (int i = 0; i < typeName.size(); i++){
    string name = typeName[i];
    XDLType* t = getSymbol(name);
    t->display(binaryIn, asciiOut); 
  } 
}
#endif
