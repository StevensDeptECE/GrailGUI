class XDLTypes {
 public:
  const static U32* u32;
  const static U16* u16;
};

void howtoservexdl() {
  DynArray<XDLType*> xdlData;  // pages accessed by number  ie getPage(0)

  XDLCompiler* compiler = new XDLCompiler(filename);
  SymbolTable* st = new SymbolTable(compiler);
  Struct* point = new Struct(
      compiler,
      "Point");  // why ever create a struct if you don't know its name?
  s->addMember(XDLType::f64, "x");
  s->addMember(XDLType::f64, "y");
  s->addMember(XDLType::f64, "z");

  Struct* s = new Struct(compiler, "bogus");
  s->addMember(
      DataType::U32,
      "a", );  // could work using enumerated type (look up in the static array)

  //  s->addMember(XDLTypes::u32, "x", );
  s->addMember(XDLTypes::u64, "b");
  s->addMember(XDLType::f64, "c");
  s->addMember(XDLType::f64, "bach");
  s->addMember(XDLType::string, "s");
  s->addMember(XDLType::strct, "Point", "p");
  s->addMember(XDLType::strct, "Point", "q");
  st->addRoot(s);
  xdlData.add(st);

  class MyThing {
   private:
    uint32_t a, b;

   public:
    void write(Buffer& buf) {
      buf.write(a);
      buf.write(b);
    }
    void writeMeta(Buffer& buf) {
      buf.write(DataType::U32);
      buf.write(DataType::U32);
    }
  };
  /* s->writeMeta(buf)
           STRUCT8 5 bogus 7
U32 1 x
U64 1 b
F64 1 c
F64 4 bach
STRING8 1 s
STRUCT8 5 point 1 p [define point here?]  3 F64 1 x F64 1 y F64 1 z
STRUCT8 5 point 1 q
   */

  st = new SymbolTable(compiler);
  point = new Struct(compiler, "Point");
  s->addMember(XDLType::f64, "x");
  s->addMember(XDLType::f64, "y");
  s->addMember(XDLType::f64, "z");

  st->addRoot(XDLType::list, point);
  xdlData.add(st);
