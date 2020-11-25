#pragma once

#include <iostream>
#include <string>
#include "util/DynArray.hh"
#include "util/HashMap.hh"

class Struct {
private:
	struct pair {
		string name; DataType type;
	}
	DynArray<pair> fields;
	HashMap<DataType> byName;
	std::string name;
public:
	Struct(const char name[]) : name(name) {}
	void add(const char name[], DataType t);
	void write(char* buf);
	void read(const char buf[]);
  friend std::ostream& operator <<(std::ostream& s, const Struct& str);


	/*
		copying a structure. This is how inheritance is done
		copy the parent, then override.
	 */
	Struct(const Struct& orig) {

	}
};

class InterpretCSPStream {
private:
	Struct metadata;
	const char* data;
public:

};

/*
describes the type of data
contains the size in bytes

*/
class Symbol {
public: // TODO should this be public?
	uint32_t size; // size of this type in bytes
	std::string name;
	std::string cppType; // corresponding type string in C++ U32 --> uint32_t
  Symbol* assignCompatibleWith; // if not null, type may be assigned from another that is internally equivalent
	
}
	
class CSPSymbolTable {
private:
	HashMap<Symbol> symbols;
public:
	CSPSymbolTable() {
		symbols.add("U8", Symbol(   ) );
		symbols.add("U16", Symbol(   ) );
    
	}
	void addSymbol(const Struct& s) {


	}
  Symbol* inherits(const char parent[], const char childname[]) {
	}

	void loadLanguage(const char langCode[]) {

	}
};










								 
