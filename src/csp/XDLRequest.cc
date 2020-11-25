#include <csp/csp.hh>
#include <cstdint>
#include <iostream>
#include <xdl/XDLCompiler.hh>
#include <xdl/std.hh>

#include "csp/XDLRequest.hh"
#include "xdl/SymbolTable.hh"

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
  st->addRoot(s);
  xdlData.add(st);

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
#if 0
  List<StockQuote> *quotes = new List<StockQuote>();
  quotes.read("IBM.dat");
  xdlData.push_back(quotes);
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

  SymbolTable* st = xdlData[requestId];
  //Struct* s = (Struct*)st->getSymbol(root);
  st->writeMeta(out);
  st->write(out);
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
