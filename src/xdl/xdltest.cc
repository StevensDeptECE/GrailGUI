#include "util/Buffer.hh"
#include "xdl/SymbolTable.hh"
#include "xdl/XDLCompiler.hh"
#include "xdl/std.hh"

class Vec3d {
private:
  double x,y,z;
public:
  Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
};

/*
TODO:
  create a definition introducer that automatically defines STRUCT with a unique ID
all later references can simply refer to the ID
  Vec3d v;
   b.write(DataType::DEFINESTRUCT);
   b.write(uint32_t(11)); //  32 bit number now refers to structure below
   v.writeMeta(b);
*/

int main() {
  XDLCompiler xdlcompiler("std.xdl");
  SymbolTable& sym = xdlcompiler.getSymbolTable();
  sym.addBuiltin("x", DataType::U8);
  std::cout<< new U8(32)<< endl;
  
  Buffer dataBuf("data.bin", 32768);
  Buffer metaBuf("metadata.bin", 32768);
  
  sym.addTypedef("UserId", "u128");
  sym.addTypedef("y", "u128");
  // note: if strict by default should not be able to assign values of type y = Userid
  
  /*
    struct Vec3d {
       double x;
       double y;
       double z;   // double x,y,z;
   }
   */
  
  
  uint32_t a = 123; 
  sym.addTypedef("a", "u32"); // 1 byte metadata
  sym.getSymbol("a")->writeMeta(metaBuf);  // 8 bytes number
  dataBuf.write(a);

  uint64_t b = 1234567;
  sym.addTypedef("b", "u64"); // 1 byte metadata
  sym.getSymbol("b")->writeMeta(metaBuf);  // 8 bytes number
  dataBuf.write(b);

  
  uint32_t d = 123; 
  sym.addValueXdl("d", new U16(32)); // 1 byte metadata
   sym.getSymbol("d")->writeMeta(metaBuf);  // 8 bytes number
  dataBuf.write(d);
  std::cout<<sym.getSymbol("d")->getTypeName()<< endl;
  metaBuf.write(sym.getSymbol("d")->getDataType());


  Struct* s = sym.addStruct("Vec3d");
  sym.addMember(s, "double", "x");
  sym.addMember(s, "double", "y");
  sym.addMember(s, "double", "z");

  
  Struct* s1 = sym.addStruct("ContactInfoTest");
  sym.addMember(s1, "Address", "address");
  sym.addMember(s1, "Phone", "phone");
  sym.addMember(s1, "Email", "email");
  
  Struct* s2 = sym.addStruct("UserTest");
  sym.addMember(s2, "UserId", "userid");
  sym.addMember(s2, "string8", "firstname");
  sym.addMember(s2, "string8", "lastname");
  sym.addMember(s2, "ContactInfo","contact");

  Vec3d v(1,2,3);
  s->writeMeta(metaBuf); // test symbol table writing metadata for Vec3d
  dataBuf.write(1.0);
  dataBuf.write(2.0);
  dataBuf.write(3.0);

  s1->writeMeta(metaBuf);
  s2->writeMeta(metaBuf);
  #if 0
  List<Vec3d> vectors;
  vectors.add(Vec3d(1,2,3));
  vectors.add(Vec3d(4,5,6));
  vectors.add(Vec3d(1,2,3));
  vectors.writeMeta(buf3); // LIST8 3 v.writeMeta(buf3);
  vectors.write(buf3); // pure data
  #endif
}

