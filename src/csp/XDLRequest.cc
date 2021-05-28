#include <csp/csp.hh>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <xdl/XDLCompiler.hh>
#include <xdl/std.hh>

#include "csp/XDLRequest.hh"
#include "xdl/SymbolTable.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

XDLRequest::XDLRequest(const char filename[]) : Request(), xdlData(3) {
  // TODO: create all the XDL objects to satisfy requests
  // TODO: for now, ignore the filename and hardcode some objects

  compiler = new XDLCompiler(filename);
  SymbolTable* st = new SymbolTable(compiler);
  Struct* s = new Struct(compiler);  // name encoded as 0""
  s->addMember("x", new U32(1));
  s->addMember("y", new U64(2));
  s->addMember("z", new F64(3.0));
  s->addMember("bach", new F64(1.23456e+5));
  s->addMember("gnuoncrack", new U32(1));
	s->addMember("", new U32(1));
  st->addRoot(s);

	xdlData.add(st);

	// load page 1
	addPage("res/aapl.bin");
#if 0
	List<Quote> *quotes = new List<StockQuote>();
  quotes.read("res/aapl.bin");
  s->addMember("quotes", quotes);
#endif


#if 0
  st = new SymbolTable(compiler);
  s = new Struct(compiler);
  s->addMember("uid", new U128((uint64_t)12345, (uint64_t)123456));
  st->addRoot(s);
  xdlData.add(st);

  st = new SymbolTable(compiler);
  st->addRoot(new ContactInfo(Address("90 W. 123 St. New York, NY 10027"),
                                 Phone(2125512638), Email("test@yahoo.com")));
  xdlData.add(st);

  // this is just metadata, no actual values (of limited utility hardcoded in a
  // server)
  st = new SymbolTable(compiler);
  // TODO: st = new SymbolTable(compiler, "fred", "DataType::U32", "alice",
  // DataType::STRING8);
  // Using template<typename... Args> (variatic template pair parameters) ---
  s = new Struct(compiler);
  s->addBuiltin("fred", DataType::U32);
  s->addBuiltin("alice", DataType::STRING8);
  st->addRoot(s);

  // Replacing this ^--------------------------------------------------------
  xdlData.add(st);
  #endif
}

void XDLRequest::handle(int fd) {
  cout << "handling XDL request" << endl;
  out.attachWrite(fd);
  in.attachRead(fd);
  in.displayRawRead();
  // buffer ..   buffer+dataSize
  // for now, hardcoded first 4 bytes of buffer is the request number
  uint32_t requestId = in.readU32();
  cout << "requestId: " << requestId << '\n';
  if (requestId >= xdlData.size()) {
    // srvlog.error(Errcode::ILLEGAL_SERVLETID);
    // commented this line out because it causes an error
    // ERROR: In function `CSPRequest::handle(int)': CSPRequest.cc:(.text+0xf6):
    // undefined reference to `srvlog'
    cout << "ERROR::ILLEGAL_SERVLETID\n";
    return;
  }

  XDLType* x = xdlData[requestId];
  //Struct* s = (Struct*)st->getSymbol(root);
  x->writeMeta(out);
  x->write(out);
  out.displayRaw();
  out.flush();
}

XDLRequest::~XDLRequest() {
  delete compiler;
  for (int i = 0; i < xdlData.size(); i++) {
    delete xdlData[i];
  }
}
void XDLRequest::handle(int, char const*) {}

void XDLRequest::addPage(const char metaDataFilename[], const char filename[]) {
	
}
#if 1
void XDLRequest::addPage(const char filename[]) {
	int fh = open(filename, binFlags);
  if (fh < 0) {
		cerr << "Error opening file " << filename << '\n';
	}
	struct stat s;
	fstat(fh, &s);
	// create an XDLRaw object with all the data in the file
	char* p = new char[s.st_size];
	ssize_t bytesRead = read(fh, p, s.st_size);
	if (bytesRead < s.st_size) {
		cerr << "Error reading file " << filename << '\n';
	}
	xdlData.add(new XDLRaw(p, s.st_size));
	close(fh);
}
#endif

// TODO: Verify that above implementation works on both Linux and Windows.
//  If so, delete below implementation of addPage
#if 0
void XDLRequest::addPage(const char filename[]) {
	ifstream in(filename, ios::binary| ios::ate);
  if (!in.good()) {
		cerr << "Error opening file " << filename << '\n';
    return;
  }
  size_t size = in.tellg();
	// create an XDLRaw object with all the data in the file
	char* p = new char[size];
	in.read(p, size);
	xdlData.add(new XDLRaw(p, size));
}
#endif 