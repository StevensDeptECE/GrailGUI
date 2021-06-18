#include "QuoteTable.hh"
#include "util/Buffer.hh"
#include "xdl/std.hh"
using namespace std;

HashMap<uint32_t> Quote::symTab(1024);

Table::Table(const char filename[]) {
	ifstream t(filename);
	t.seekg(0, ios::end);
	uint32_t len = t.tellg();
	data = new unsigned char[len];
	t.seekg(0, std::ios::beg);
	t.read((char*)data, len);
	uint32_t numQuotes = *(uint32_t*)data; // first 4 bytes are the #of quotes
	quotes = (const Quote*)(data + sizeof(uint32_t));	
}
// 14080           19620102.........
// len len len len quote quote quote quote ....

void Table::loadASCII(const char textFile[], const char binFile[]) {
	ifstream in(textFile);
	in.seekg(0, ios::end);
	uint32_t len = in.tellg();
	in.seekg(0, std::ios::beg);	
	char buf[4096];
	DynArray<Quote> tempQuotes(len / 30);
	in.getline(buf, sizeof(buf)); // throw out first line of metadata
	while (in.getline(buf, sizeof(buf))) {
		Quote q(buf);
		tempQuotes.add(q);
	}
	Buffer out(binFile, 32768);
  /**
   * TODO:
   * SymbolTable st(compiler);
   * st->addXDLType(tempQuotes[0]);
   * Struct* s = new Struct();
   * List16<Quote>* quotes = new List16<Quote>(15000); // preallocate big enough list
   * quotes->add the data here....
   * 
   * s->addMember("quotes", quotes);
   * st->addXDLType(root, s);
   * out.write(st);
   */
  out.write(uint8_t(2)); // total number of elements in the symbol table (would be nice to use SymbolTable to generate this later)
  tempQuotes[0].writeMeta(out); // first item, define "quote"
  out.write(DataType::STRUCT8, "root"); // then define "root" that is the top level object 
  out.write(uint8_t(1)); // 1 member in root (a list of quotes)
  out.write(DataType::LIST16, "quotes"); // a list called quotes, maximum 65535 elements
  out.write(uint16_t(tempQuotes.size())); // the size of the list (about 14k)
  out.write("quote", 5); // the type of the object in the list
  //write metadata first

  for (uint32_t i = 0; i < tempQuotes.size(); i++) {
    tempQuotes[i].write(out);
  }

	string binFile2 = string(binFile);
	binFile2 = binFile2.substr(0, binFile2.length()-4) + "2.bin";
	Buffer out2(binFile2.c_str(), 1024*1024);
  /**
   * TODO:
   * SymbolTable st(compiler);
   * st->addXDLType(tempQuotes[0]);
   * Struct* s = new Struct();
   * List16<Quote>* quotes = new List16<Quote>(15000); // preallocate big enough list
   * quotes->add the data here....
   * 
   * s->addMember("quotes", quotes);
   * st->addXDLType(root, s);
   * out.write(st);
   */
  out.write(uint8_t(2)); // total number of elements in the symbol table (would be nice to use SymbolTable to generate this later)
  tempQuotes[0].writeMeta(out); // first item, define "quote"
  out.write(DataType::STRUCT8, "root"); // then define "root" that is the top level object 
  out.write(uint8_t(1)); // 1 member in root (a list of quotes)
  out.write(DataType::LIST16, "quotes"); // a list called quotes, maximum 65535 elements
  out.write(uint16_t(tempQuotes.size())); // the size of the list (about 14k)
  out.write("quote", 5); // the type of the object in the list
  //write metadata first

	tempQuotes[0].write(out);
  for (uint32_t i = 1; i < tempQuotes.size(); i++) {
		DeltaQuoteNoDate* dq = (DeltaQuoteNoDate*)&tempQuotes[i];
    dq->write(dq[-1], out);
  }

}
