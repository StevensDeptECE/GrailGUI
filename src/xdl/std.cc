
#include "xdl/std.hh"

#include "opengl/Canvas.hh"
#include "opengl/MultiText.hh"
#include "opengl/Style.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "util/Buffer.hh"
#include "util/Ex.hh"
#include "util/datatype.hh"
#include "xdl/XDLCompiler.hh"
using namespace std;

DynArray<string> XDLType::typeNames(1024);
DynArray<const XDLType*> XDLType::types(1024);
const string XDLType::empty = "";
HashMap<uint32_t> XDLType::byName(1024);
const UnImpl* XDLType::unimpl = nullptr;

inline const XDLType* XDLType::addType(const XDLType* type) { return type; }

void writeMeta(Buffer& buf, const XDLType& data) {
  buf.write(data.getDataType());
}

DataType XDLType::readType(Buffer& in) { return DataType(in.readU8()); }

void XDLType::display(Buffer& binaryIn, Buffer& asciiOut) const {}

// this must be called before XDLType is used (class initialization of static
// vars)
void XDLType::classInit() {
  addType(new U8());
  addType(new U16());
  addType(new U24());
  addType(new U32());
  addType(new U64());
  addType(new U128());
  addType(new U256());
  addType(new I8());
  addType(new I16());
  addType(new I24());
  addType(new I32());
  addType(new I64());
  addType(new I128());
  addType(new I256());
  addType(new F32());
  addType(new F64());
  addType(new Bool());
  addType(new Date());
  addType(new JulianDate());
  addType(new Timestamp());
  addType(new String8());
  unimpl = static_cast<const UnImpl*>(addType(new UnImpl()));
  //  addType(new String16());
  // addType(new String32());
  // addType(new String64());
  // addType(new UTF8_8());
  // addType(new UTF8_16());
  // addType(new UTF8_32());
  // addType(new UTF8_64());
  // addType(new UTF16_8());
  // addType(new UTF16_16());
  // addType(new UTF16_32());
  // addType(new UTF16_64());
  // addType(new Regex());
  // addType(new UTF8_16());
  // addType(new UTF8_32());
  // addType(new UTF8_64());
  // addType(new GenericList());
  // addType(new GenericList());
  // addType(new GenericList());
  // addType(new GenericList());
  // addType(new Struct());
  // addType(new Struct());
  // addType(new Struct());
  // addType(new Struct()); //TODO: DynamicList
  // addType(new Struct());
  // addType(new Func1());
  // addType(new Func2());
  // addType(new FuncParam1());
  // addType(new FuncParam2());
  // Why bother with these, we won't get to them for a LONG
  // time...
  // BIGINT
}

void XDLType::classCleanup() {
  for (uint32_t i = 0; i < types.size(); i++) {
    if (types[i] != nullptr) {
      delete types[i];
      types[i] = nullptr;
    }
  }
}

DataType XDLRaw::getDataType() const {
  return DataType::U8;  // TODO: add this type!
}

uint32_t XDLRaw::size() const { return len; }

void write(Buffer& buf, const XDLRaw& r) { buf.write(r); }

void XDLRaw::display(Buffer& binaryIn, Buffer& asciiOut) const {}

void XDLRaw::format(Buffer& binaryIn, Buffer& asciiOut,
                    const char fmt[]) const {}

void Struct::addSym(const string& memberName, const XDLType* t) {
  byName.add(memberName.c_str(), types.size());
  members.add(Member(memberNames.length(), memberName.length(), t));
  memberNames += memberName;
}

void Struct::addSymCheckNull(const string& name, const XDLType* t2) {
  const XDLType* t = getMemberType(name);
  if (t == nullptr) {  // undefined symbol
    compiler->undefinedSymbol(name);
    // throw Ex2(Errcode::UNDEFINED_TYPE, string("Type : ") + name);
    return;
  }
  addSym(name, t2);
}

void Struct::addSymCheckDup(const string& name, const XDLType* t2) {
  const XDLType* t = getMemberType(name);
  if (t != nullptr) {  // undefined symbol
    compiler->duplicateSymbol(name);
    return;
  }
  addSym(name, t2);
}

void Struct::addMember(const string& memberName, const XDLType* type) {
  if (type == nullptr) {
    compiler->undefinedSymbol(memberName);
    // throw Ex2(Errcode::UNDEFINED_TYPE, string("Type : " + name));
    return;
  }
  byName.add(
      memberName.c_str(),
      members
          .size());  // size of the array will be the index of the new element
  members.add(Member(memberNames.length(), memberName.length(),
                     type));   // uses copy constructor (placement new)
  packedSize += type->size();  // add up the packed byte size of all members
  memberNames += memberName;
}

void Struct::addBuiltin(const string& memberName, DataType t) {
  addMember(memberName, types[uint32_t(t)]);
}

void Struct::addTypedef(const char name[], const char type[]) {
  const XDLType* t = getMemberType(type);
  if (t == nullptr) {  // undefined symbol
    compiler->undefinedSymbol(name);
    // throw Ex2(Errcode::UNDEFINED_TYPE, string("Type : ") + name);
    return;
  }
  addSymCheckDup(name, new TypeDef(name, t));
}

#if 0
void Struct::addRegex(const string& name, const string& regex) {
  addSymCheckDup(name, new Regex(name, regex));
}
#endif

inline void Struct::addStructMember(const string& memberName,
                                    const Struct* memberStruct) {
  members.add(Member(memberNames.length(), memberName.length(), memberStruct));
}

// TODO: implement!
const Struct* XDLType::read(Buffer& in) { return nullptr; }

void Struct::addData(ArrayOfBytes* data) const {
  for (int i = 0; i < members.size(); i++) {
    members[i].type->addData(data);
  }
}

void Struct::addMeta(ArrayOfBytes* meta) const {
  meta->addMeta(this->getDataType());
  for (int i = 0; i < members.size(); i++) {
    members[i].type->addMeta(meta);
  }
}

/* Reads Buffer into struct s*/
const XDLType* XDLType::readMeta(XDLCompiler* compiler, Buffer& in) {
  DataType t = in.readType();
  switch (t) {
    case DataType::U8:
    case DataType::U16:
    case DataType::U32:
    case DataType::U64:
    case DataType::U128:
    case DataType::U256:
    case DataType::I8:
    case DataType::I16:
    case DataType::I32:
    case DataType::I64:
    case DataType::I128:
    case DataType::I256:
    case DataType::BOOL:
    case DataType::F32:
    case DataType::F64:
    case DataType::DATE:
    case DataType::JULDATE:
    case DataType::STRING8: {
      return XDLType::getBuiltinType(t);
      break;
    }
    case DataType::STRUCT8: {
      string name = in.readString8();
      uint8_t numMembers = in.readU8();
      Struct* s = new Struct(compiler, name);
      string memberName;
      for (int i = 0; i < numMembers; i++) {
        const XDLType* member = readMeta(compiler, in);
        memberName = in.readString8();
        s->addSymCheckDup(memberName, member);
      }
      return s;
    }
    // case DataType::STRUCT16:
    // case DataType::STRUCT32
    case DataType::LIST8:
    case DataType::LIST16:
    case DataType::LIST32:
    case DataType::LIST64: {
      string name = in.readString8();
      GenericList* genList =
          new GenericList(compiler, name, readMeta(compiler, in));
      return genList;
    }
      //        case DataType::List16:
      //        case DataType::List32
  }
  return unimpl;
}

// XDLIterator::XDLIterator(XDLType* underlying)
//    : XDLType("Iterator"), underlying(underlying) {}

DataType U8::getDataType() const { return DataType::U8; }
uint32_t U8::size() const { return 1; }
void write(Buffer& buf, const U8& data) { buf.write(data.val); }
void U8::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU8(binaryIn.readU8());
}
#if 0
void U8::display(Buffer& in, Canvas* c, const Style* s, float x0, float y0,
                 float* w, float* h) const {
  StyledMultiShape2D* m = (StyledMultiShape2D*)c->getLayer(0);
  MultiText* t = (MultiText*)c->getLayer(1);
  const Font* f = s->f;
  t->add(x0, y0, f, in.readU8());
}
#endif

DataType U16::getDataType() const { return DataType::U16; }
uint32_t U16::size() const { return 2; }
void write(Buffer& buf, const U16& data) { buf.write(data.val); }
void U16::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU16(binaryIn.readU16());
}
DataType U24::getDataType() const { return DataType::U24; }
uint32_t U24::size() const { return 3; }
void write(Buffer& buf, const U24& data) { buf.write(data.val); }
void U24::display(Buffer& binaryIn, Buffer& asciiOut) const {
  // asciiOut.append(binaryIn.readU24());
}

DataType U32::getDataType() const { return DataType::U32; }
uint32_t U32::size() const { return 4; }
void write(Buffer& buf, const U32& data) { buf.write(data.val); }
void U32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU32(binaryIn.readU32());
}

#if 0
void U32::display(Buffer& in, MultiShape2d* m, MultiText* t, const Style* s,
									float x0, float y0,
                  float* w, float* h) const {
  const Font* f = s->f;
  t->add(x0, y0, f, in.readU32());
}
#endif

DataType U64::getDataType() const { return DataType::U64; }
uint32_t U64::size() const { return 8; }
void write(Buffer& buf, const U64& data) { buf.write(data.val); }
void U64::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU64(binaryIn.readU64());
}

#if 0
void U64::display(Buffer& in, Canvas* c, const Style* s, float x0, float y0,
                  float* w, float* h) const {
  StyledMultiShape2D* m = (StyledMultiShape2D*)c->getLayer(0);
  MultiText* t = (MultiText*)c->getLayer(1);
  const Font* f = s->f;
  // TODO: t->add(x0, y0, f, in.readU64());
}
#endif

DataType U128::getDataType() const { return DataType::U128; }
uint32_t U128::size() const { return 16; }
void write(Buffer& buf, const U128& data) {
  buf.write(data.a);
  buf.write(data.b);
}
void U128::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU64(binaryIn.readU64());
  asciiOut.appendU64(binaryIn.readU64());
}

DataType U256::getDataType() const { return DataType::U256; }
uint32_t U256::size() const { return 32; }
void write(Buffer& buf, const U256& data) {
  buf.write(data.a);
  buf.write(data.b);
  buf.write(data.c);
  buf.write(data.d);
}
void U256::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU64(binaryIn.readU64());
  asciiOut.appendU64(binaryIn.readU64());
  asciiOut.appendU64(binaryIn.readU64());
  asciiOut.appendU64(binaryIn.readU64());
}

DataType I8::getDataType() const { return DataType::I8; }
uint32_t I8::size() const { return 1; }
void write(Buffer& buf, const I8& data) { buf.write(data.val); }
void I8::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendI8(binaryIn.readI8());
}

DataType I16::getDataType() const { return DataType::I16; }
uint32_t I16::size() const { return 2; }
void write(Buffer& buf, const I16& data) { buf.write(data.val); }
void I16::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendI16(binaryIn.readI16());
}

DataType I24::getDataType() const { return DataType::I24; }
uint32_t I24::size() const { return 3; }
void write(Buffer& buf, const I24& data) { buf.write(data.val); }
void I24::display(Buffer& binaryIn, Buffer& asciiOut) const {
  // int32_t I24 = binaryIn.readI24();
  // asciiOut.appendAscii(v);
}

DataType I32::getDataType() const { return DataType::I32; }
uint32_t I32::size() const { return 4; }
void write(Buffer& buf, const I32& data) { buf.write(data.val); }
void I32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendI32(binaryIn.readI32());
}

DataType I64::getDataType() const { return DataType::I64; }
uint32_t I64::size() const { return 8; }
void write(Buffer& buf, const I64& data) { buf.write(data.val); }
void I64::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendI64(binaryIn.readI64());
}

DataType I128::getDataType() const { return DataType::I128; }
uint32_t I128::size() const { return 16; }
void write(Buffer& buf, const I128& data) {
  buf.write(data.a);
  buf.write(data.b);
}
void I128::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendI64(binaryIn.readI64());
  asciiOut.appendI64(binaryIn.readI64());
}

DataType I256::getDataType() const { return DataType::I256; }
uint32_t I256::size() const { return 32; }
void write(Buffer& buf, const I256& data) {
  buf.write(data.a);
  buf.write(data.b);
  buf.write(data.c);
  buf.write(data.d);
}
void I256::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendI64(binaryIn.readI64());
  asciiOut.appendI64(binaryIn.readI64());
  asciiOut.appendI64(binaryIn.readI64());
  asciiOut.appendI64(binaryIn.readI64());
}

DataType Bool::getDataType() const { return DataType::BOOL; }
uint32_t Bool::size() const { return 1; }
void write(Buffer& buf, const Bool& data) { buf.write(data.val); }
void Bool::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU8(binaryIn.readU8());
}

DataType F32::getDataType() const { return DataType::F32; }
uint32_t F32::size() const { return 8; }
void write(Buffer& buf, const F32& data) { buf.write(data.val); }
void F32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendF32(binaryIn.readF32());
}

DataType F64::getDataType() const { return DataType::F64; }
uint32_t F64::size() const { return 8; }
void write(Buffer& buf, const F64& data) { buf.write(data.val); }
void F64::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendF64(binaryIn.readF64());
}

#if 0
void F64::display(Buffer& in, Canvas* c, const Style* s, float x0, float y0,
                  float* w, float* h) const {
  StyledMultiShape2D* m = (StyledMultiShape2D*)c->getLayer(0);
  MultiText* t = (MultiText*)c->getLayer(1);
  const Font* f = s->f;
  t->add(x0, y0, f, in.readF64());
}
#endif

DataType Date::getDataType() const { return DataType::DATE; }
uint32_t Date::size() const { return 4; }
void write(Buffer& buf, const Date& data) { buf.write(data.date); }
void Date::display(Buffer& binaryIn, Buffer& asciiOut) const {
  Date d(binaryIn.readU32());
  // TODO: support many formats
  asciiOut.appendU32(d.getYear());
  // asciiOut.append(' ');
  asciiOut.appendU32(d.getMonth());
  // asciiOut.append(' ');
  asciiOut.appendU32(d.getDay());
}

int32_t Date::getYear() const {
  int32_t dy = int32_t(date / 365.2425);
  return 2000 + dy;
}

uint32_t Date::getMonth() const { return 0; }

uint32_t Date::getDay() const { return 0; }

// TODO: better naming convention than ddate
int32_t JulianDate::getYear() const {
  int64_t ddate = int64_t(date);
  int32_t dy = ddate / 365 + ddate / 4 - ddate / 100 + ddate / 400;
  std::cout << dy;
  std::cout << (dy = ddate / 365.2425);
  return epoch + dy;
  //    uint32_t month = (E < 13.5) ? E - 1 : E - 13;
  //    return (month > 2.5) ? C - 4716 : C - 4715;
}
uint32_t JulianDate::getMonth() const {
  double ddate = floor(date);
  int32_t mm = ddate - floor(floor(ddate / 365.2425) * 365.2425);
  return mm;
  //     return (E < 13.5) ? E - 1 : E - 13;
}
uint32_t JulianDate::getDay() const {
  double ddate = floor(date);
  int32_t dd = ddate - floor(floor(ddate / 365.2425) * 365.2425);
  return dd;  // TODO: this is completely wrong
}
uint32_t JulianDate::getHour() const {
  double frac = date - floor(date);
  return uint32_t(frac * 24);
}
uint32_t JulianDate::getMinute() const {
  double frac = date - floor(date);
  return uint32_t(frac * (24 * 60)) % 60;
}
double JulianDate::getSecond() const {
  double frac = date - floor(date);
  return uint32_t(frac * (24 * 60 * 60)) % 60;
}

DataType JulianDate::getDataType() const { return DataType::JULDATE; }
uint32_t JulianDate::size() const { return 8; }
void write(Buffer& buf, const JulianDate& data) { buf.write(data.date); }
void JulianDate::display(Buffer& binaryIn, Buffer& asciiOut) const {
  JulianDate d(binaryIn.readF64());
  // TODO: FIx format!!!
  asciiOut.appendU32(d.getYear());
  asciiOut.appendU32(d.getMonth());
  asciiOut.appendU32(d.getDay());
}

void write(Buffer& b, const Timestamp& data) {
  throw Ex1(Errcode::UNIMPLEMENTED);
}

void writeMeta(Buffer& buf, const Timestamp& data) {
  throw Ex1(Errcode::UNIMPLEMENTED);
}

uint32_t Timestamp::size() const { return 8; }

DataType Timestamp::getDataType() const { return DataType::TIMESTAMP; }

void Timestamp::display(Buffer& binaryIn, Buffer& asciiOut) const {}
void Timestamp::format(Buffer& binaryIn, Buffer& asciiOut,
                       const char fmt[]) const {}

DataType String8::getDataType() const { return DataType::STRING8; }
uint32_t String8::size() const { return 1 + val.length(); }
void write(Buffer& buf, const String8& data) { buf.write(data.val); }
void String8::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string v = binaryIn.readString8();
  asciiOut.append(v.c_str());
}

DataType String16::getDataType() const { return DataType::STRING16; }
uint32_t String16::size() const { return 2 + val.length(); }
void write(Buffer& buf, const String16& data) { buf.write(data.val); }
void String16::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asm("int3");  // TODO: implement me!
}

DataType String32::getDataType() const { return DataType::STRING32; }
uint32_t String32::size() const { return 4 + val.length(); }
void write(Buffer& buf, const String32& data) { buf.write(data.val); }
void String32::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asm("int3");  // TODO: implement me!
}

DataType String64::getDataType() const { return DataType::STRING64; }
uint32_t String64::size() const { return 8 + val.length(); }
void write(Buffer& buf, const String64& data) { buf.write(data.val); }
void String64::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asm("int3");  // TODO: implement me!
}

/*
        All code below here will eventually be generated by the compiler
and then this will be deleted.
*/
#if 0
void UserId::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(typeName);
}

void UserId::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint64_t v = binaryIn.readU64();
  asciiOut.append("User id");
  asciiOut.appendU64(v);
}

void Address::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(typeName);
}

void Address::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string v = binaryIn.readString8();
  // TODO: asciiOut.append(v);
}

void Phone::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(typeName);
}

void Phone::display(Buffer& binaryIn, Buffer& asciiOut) const {
  asciiOut.appendU64(binaryIn.readU64());
}

DataType Email::getDataType() const { return DataType::STRING8; }

uint32_t Email::size() const {
  return 16;  // TODO: compute size
}

void Email::write(Buffer& buf) const { buf.write(val); }
void Email::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(getTypeName());
}

void Email::display(Buffer& binaryIn, Buffer& asciiOut) const {
  string v = binaryIn.readString8();
  asciiOut.append(v.c_str());
}

DataType ContactInfo::getDataType() const { return DataType::STRUCT8; }

uint32_t ContactInfo::size() const {
  return address.size() + phone.size() + email.size();
}

void ContactInfo::write(Buffer& buf) const {
  address.write(buf);
  phone.write(buf);
  email.write(buf);
}

void ContactInfo::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(getTypeName());
  buf.write(uint8_t(3));
  address.writeMeta(buf);
  phone.writeMeta(buf);
  email.writeMeta(buf);
}

void ContactInfo::display(Buffer& binaryIn, Buffer& asciiOut) const {
  address.display(binaryIn, asciiOut);
  phone.display(binaryIn, asciiOut);
  email.display(binaryIn, asciiOut);
}

DataType User::getDataType() const { return DataType::STRUCT8; }

uint32_t User::size() const {
  return 12;  // TODO: constant + sizeof component strings
}

void User::write(Buffer& buf) const {
  buf.checkSpace(sizeof(*this));
  userid.write(buf);
  buf.write(firstname);
  buf.write(lastname);
  contact.write(buf);
}

void User::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(getTypeName());
  buf.write(uint8_t(4));
  userid.writeMeta(buf);
  buf.write(DataType::STRING8);
  string fn = "FirstName";
  buf.write(fn);
  buf.write(DataType::STRING8);
  string ln = "LastName";
  buf.write(ln);
  contact.writeMeta(buf);
}

void User::display(Buffer& binaryIn, Buffer& asciiOut) const {
  userid.display(binaryIn, asciiOut);
  string v_fn = binaryIn.readString8();
  asciiOut.append(v_fn.c_str());
  string v_ln = binaryIn.readString8();
  asciiOut.append(v_ln.c_str());
  contact.display(binaryIn, asciiOut);
}
#endif

uint32_t Struct::size() const { return packedSize; }

void write(Buffer& buf, const Struct& s) {
  for (uint32_t i = 0; i < s.members.size(); i++) {
    s.members[i].type->writeXDL(buf);  // each member write metadata
  }
}

void writeMeta(Buffer& buf, const Struct& s) {
  buf.write(s.getDataType());
  buf.write(s.getTypeName());            // name
  buf.write(uint8_t(s.members.size()));  // TODO: this is hardcoded to 8-bit so
                                         // no more than 255 members
  for (uint32_t i = 0; i < s.members.size(); i++) {
    s.members[i].type->writeXDLMeta(buf);  // each member write metadata
    buf.write(&(s.memberNames[s.members[i].nameOffset]), s.members[i].nameLen);
  }
}

void Struct::display(Buffer& binaryIn, Buffer& asciiOut) const {
  for (int i = 0; i < members.size(); i++) {
    const Member& m = members[i];
    asciiOut.append(&memberNames[m.nameOffset], m.nameLen);
    asciiOut.write(' ');
    m.type->display(binaryIn, asciiOut);
    asciiOut.write('\n');
  }
}

void Struct::format(Buffer& binaryIn, Buffer& asciiOut,
                    const char fmt[]) const {}

// const XDLType* Struct::getMember(uint32_t i) const { return
// members.at(i).type; }

uint32_t Struct::getMemberCount() const { return members.size(); }

DataType GenericList::getDataType() const { return DataType::LIST16; }

void write(Buffer& buf, const GenericList& list) {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
uint32_t GenericList::size() const { return 0; }

void writeMeta(Buffer& buf, const GenericList& list) {
  buf.write(list.getDataType());
  buf.write(list.getTypeName());
  buf.write(list.listType);
  // TODO: check for overflow of buffer!
}

void GenericList::display(Buffer& binaryIn, Buffer& asciiOut) const {
  uint64_t size = binaryIn.readU16();
  for (int i = 0; i < size; i++) {
    listType->display(binaryIn, asciiOut);
  }
}

XDLType* GenericList::begin(Buffer& buf) { return new Iterator(this, buf); }

uint32_t TypeDef::size() const { return type->size(); }

void write(Buffer& buf, const TypeDef& data) { data.type->writeXDL(buf); }

void writeMeta(Buffer& buf, const TypeDef& data) {
  data.type->writeXDLMeta(buf);
}

void TypeDef::display(Buffer& binaryIn, Buffer& asciiOut) const {}

void TypeDef::format(Buffer& binaryIn, Buffer& asciiOut,
                     const char fmt[]) const {}

/*
uint32_t BuiltinType::size() const {
  return 0;  // TODO: this is unclean! We do this some other way for all
             // builtin types, but if we just did it here the code would
             // simplify
}*/

// XDLType* BuiltinType::begin(Buffer& buf) { return this; }
XDLType* XDLRaw::begin(Buffer& buf) { return this; }

/*void BuiltinType::write(Buffer& buf) const {
  // buf.write(val);
}

void BuiltinType::writeMeta(Buffer& buf) const {
  buf.write(t);  // DataType
}

void BuiltinType::display(Buffer& binaryIn, Buffer& asciiOut) const {}

void BuiltinType::format(Buffer& binaryIn, Buffer& asciiOut,
                         const char fmt[]) const {}
*/
// Currently removed, not used in anything
#if 0
Regex::Regex(const string& name, const string& exp)
    : CompoundType(name), rexp(exp) {}

void write(Buffer& buf,) const { buf.write(rexp); }

void Regex::writeMeta(Buffer& buf) const {
  buf.write(getDataType());
  buf.write(getTypeName());
}

XDLType* Regex::begin(Buffer& buf) { return this; }

void Regex::display(Buffer& binaryIn, Buffer& asciiOut) const {}

void Regex::format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {
}

uint32_t Regex::size() const { return 0; }

DataType Regex::getDataType() const { return DataType::REGEX; }
#endif
uint32_t UnImpl::size() const { return 0; }

void write(Buffer& buf, const UnImpl&) { throw Ex1(Errcode::UNIMPLEMENTED); }

void writeMeta(Buffer& buf, const UnImpl&) {
  throw Ex1(Errcode::UNIMPLEMENTED);
}

DataType UnImpl::getDataType() const { return DataType::UNIMPL; }

void XDLBuiltinType::addMeta(ArrayOfBytes* meta) const { meta->addMeta(t); }
XDLType* XDLBuiltinType::begin(Buffer& buf) {
  return nullptr;  // TODO: this will crash if you don't return an actual
                   //  object
}

void write(Buffer& b, const ArrayOfBytes& array) {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
uint32_t ArrayOfBytes::size() const { return data.size() + metadata.size(); }
DataType ArrayOfBytes::getDataType() const { return DataType::STRUCT16; }

void U8::addData(ArrayOfBytes* data) const { data->addData(val); }
void U16::addData(ArrayOfBytes* data) const { data->addData(val); }
void U24::addData(ArrayOfBytes* data) const { data->addData(val); }
void U32::addData(ArrayOfBytes* data) const { data->addData(val); }
void U64::addData(ArrayOfBytes* data) const { data->addData(val); }
void U128::addData(ArrayOfBytes* data) const {
  data->addData(a);
  data->addData(b);
}
void U256::addData(ArrayOfBytes* data) const {
  data->addData(a);
  data->addData(b);
  data->addData(c);
  data->addData(d);
}
void I8::addData(ArrayOfBytes* data) const { data->addData(val); }
void I16::addData(ArrayOfBytes* data) const { data->addData(val); }
void I24::addData(ArrayOfBytes* data) const { data->addData(val); }
void I32::addData(ArrayOfBytes* data) const { data->addData(val); }
void I64::addData(ArrayOfBytes* data) const { data->addData(val); }
void I128::addData(ArrayOfBytes* data) const {
  data->addData(a);
  data->addData(b);
}
void I256::addData(ArrayOfBytes* data) const {
  data->addData(a);
  data->addData(b);
  data->addData(c);
  data->addData(d);
}
void Bool::addData(ArrayOfBytes* data) const { data->addData(val); }
void F32::addData(ArrayOfBytes* data) const { data->addData(val); }
void F64::addData(ArrayOfBytes* data) const { data->addData(val); }
void Date::addData(ArrayOfBytes* data) const { data->addData(date); }
void JulianDate::addData(ArrayOfBytes* data) const { data->addData(date); }
void Timestamp::addData(ArrayOfBytes* data) const { data->addData(val); }
void String8::addData(ArrayOfBytes* data) const { data->addData(val); }
void String16::addData(ArrayOfBytes* data) const { data->addData(val); }
void String32::addData(ArrayOfBytes* data) const { data->addData(val); }
void String64::addData(ArrayOfBytes* data) const { data->addData(val); }
void UnImpl::addMeta(ArrayOfBytes* meta) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void UnImpl::addData(ArrayOfBytes* data) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void TypeDef::addData(ArrayOfBytes* data) const { type->addData(data); }
void TypeDef::addMeta(ArrayOfBytes* meta) const {
  meta->addMeta(DataType::TYPEDEF);
  meta->addMeta(getTypeName());
  type->addMeta(meta);
}
#if 0
void Regex::addData(ArrayOfBytes* data) const { data->addData(rexp); }
void Regex::addMeta(ArrayOfBytes* meta) const {
  meta->addMeta(DataType::REGEX);
  meta->addMeta(getTypeName());
}
#endif

// TODO: Does any CompoundType except for Struct use this?
// If not, abstract it out
void XDLRaw::addMeta(ArrayOfBytes* meta) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void XDLRaw::addData(ArrayOfBytes* data) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void GenericList::addMeta(ArrayOfBytes* meta) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void GenericList::addData(ArrayOfBytes* data) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void Calendar::addMeta(ArrayOfBytes* meta) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void Calendar::addData(ArrayOfBytes* data) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void Calendar::writeXDL(Buffer& buf) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
void Calendar::writeXDLMeta(Buffer& buf) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
uint32_t Calendar::size() const { throw Ex1(Errcode::UNIMPLEMENTED); }
XDLType* Calendar::begin(Buffer& buf) { throw Ex1(Errcode::UNIMPLEMENTED); }
void ArrayOfBytes::addStruct(const char name[], uint8_t numElements) {
  addMeta(DataType::STRUCT8);
  addMeta(name);
  metadata.add(numElements);
}

void ArrayOfBytes::addBuiltinMember(DataType t, const char str[]) {
  addMeta(t);
  addMeta(str);
}

int32_t Date::epochYear = 2000;
Date::Date(const JulianDate& jd)
    : XDLBuiltinType("Date", DataType::DATE), date(uint32_t(double(jd))) {}
Date::operator JulianDate() const { return JulianDate(date); }
Date::Date(int32_t year, uint32_t month, uint32_t day)
    : XDLBuiltinType("Date", DataType::DATE) {
  if (month < 1 || month > 12 || day < 1 ||
      day > JulianDate::daysInMonth[day]) {
    throw Ex1(Errcode::BAD_DATE);
  }

  int32_t dY = year - epochYear;
  int32_t leapYears = dY / 4 - dY / 100 + dY / 400;  // is this true
  date = dY * 365 + leapYears + JulianDate::daysUpTo[month - 1] + day;
}
const double JulianDate::epoch = 2000;

const char* JulianDate::monthNames[12] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};
const char* JulianDate::monthAbbr[12] = {"Jan", "Feb", "Mar", "Apr",
                                         "May", "Jun", "Jul", "Aug",
                                         "Sep", "Oct", "Nov", "Dec"};

const uint16_t JulianDate::daysUpTo[12] = {
    0,    // Jan
    31,   // Feb
    59,   // Mar
    90,   // Apr
    120,  // May
    151,  // Jun
    181,  // Jul
    212,  // Aug
    243,  // Sep
    273,  // Oct
    304,  // Nov
    334   // Dec
};
const uint16_t JulianDate::daysInMonth[12] = {
    31,  // Jan
    28,  // Feb
    31,  // Mar
    30,  // Apr
    31,  // May
    30,  // Jun
    31,  // Jul
    31,  // Aug
    30,  // Sep
    31,  // Oct
    30,  // Nov
    31   // Dec
};
JulianDate::JulianDate(int32_t year, uint32_t month, uint32_t day,
                       uint32_t hour, uint32_t min, uint32_t second)
    : XDLBuiltinType("JulDate", DataType::JULDATE) {
  if (month < 1 || month > 12 || day < 1 ||
      day > daysInMonth[month - 1] &&
          !(month == 2 && day == 29 && isLeap(year))) {
    throw Ex1(Errcode::BAD_DATE);
  }

  int32_t dY = year - epoch;
  int32_t leapYears = dY / 4 - dY / 100 + dY / 400;  // is this true
  int32_t extraDay = isLeap(year) && month > 2;
  date = dY * 365 + leapYears + daysUpTo[month - 1] + int32_t(day) +
         (hour * 3600 + min * 60 + second) * invSecondsPerDay + extraDay - 1;
}
void Calendar::setHoliday(const Date& d) { holidays.push_back(d); }
void Calendar::setHoliday(const Date& from, const Date& to) {
  for (Date d = from; d <= to; d++) {
    holidays.push_back(d);
  }
}

void Calendar::setRepeatingHoliday(int32_t yearStart, int32_t yearEnd,
                                   uint32_t month, uint32_t day) {}

bool Calendar::isHoliday(const Date& d) const { return false; }
Date Calendar::addBusinessDays(const Date& d, int32_t delta) const {
  return d + delta;
}
int32_t Calendar::businessDaysBetween(const Date& d1, const Date& d2) const {
  return d2 - d1;
}

/*
uint32_t fieldSize(const U8&) { return 3; }
uint32_t fieldSize(const U16&) { return 5; }
uint32_t fieldSize(const U24&){ return 8; }
uint32_t fieldSize(const U32&) { return 10; }
uint32_t fieldSize(const U64&) { return 20; }
uint32_t fieldSize(const U128&) { return 20; }
uint32_t fieldSize(const U256&) { return 20; }
uint32_t I8::fieldSize() const { return 4; }
uint32_t I16::fieldSize() const { return 6; }
uint32_t I24::fieldSize() const { return 8; }
uint32_t I32::fieldSize() const { return 11; }
uint32_t I64::fieldSize() const { return 20; }
uint32_t I128::fieldSize() const { return 20; }
uint32_t I256::fieldSize() const { return 20; }
uint32_t F32::fieldSize() const { return 14; }
uint32_t F64::fieldSize() const { return 22; }
uint32_t Bool::fieldSize() const { return 5; }
uint32_t Date::fieldSize() const { return 11; }
uint32_t JulianDate::fieldSize() const { return 22; }
uint32_t Timestamp::fieldSize() const { return 22; }
uint32_t String8::fieldSize() const { return 22; }
uint32_t String16::fieldSize() const { return 22; }
uint32_t String32::fieldSize() const { return 22; }
uint32_t String64::fieldSize() const { return 22; }
uint32_t UnImpl::fieldSize() const { return 0; }
uint32_t TypeDef::fieldSize() const { return 0; }

// TODO: write function to calculate max size when Struct is made
uint32_t Struct::fieldSize() const { return 22; }  // return maxFieldSize;
} uint32_t GenericList::fieldSize() const { return listType->fieldSize();
}

uint32_t XDLRaw::fieldSize() const { return 0; }
*/

void U8::writeXDL(Buffer& buf) const { write(buf, val); }
void U16::writeXDL(Buffer& buf) const { write(buf, val); }
void U24::writeXDL(Buffer& buf) const { write(buf, val); }
void U32::writeXDL(Buffer& buf) const { write(buf, val); }
void U64::writeXDL(Buffer& buf) const { write(buf, val); }
void U128::writeXDL(Buffer& buf) const {
  write(buf, a);
  write(buf, b);
}
void U256::writeXDL(Buffer& buf) const {
  write(buf, a);
  write(buf, b);
  write(buf, c);
  write(buf, d);
}
void I8::writeXDL(Buffer& buf) const { write(buf, val); }
void I16::writeXDL(Buffer& buf) const { write(buf, val); }
void I24::writeXDL(Buffer& buf) const { write(buf, val); }
void I32::writeXDL(Buffer& buf) const { write(buf, val); }
void I64::writeXDL(Buffer& buf) const { write(buf, val); }
void I128::writeXDL(Buffer& buf) const {
  write(buf, a);
  write(buf, b);
}
void I256::writeXDL(Buffer& buf) const {
  write(buf, a);
  write(buf, b);
  write(buf, c);
  write(buf, d);
}
void F32::writeXDL(Buffer& buf) const { write(buf, val); }
void F64::writeXDL(Buffer& buf) const { write(buf, val); }
void Bool::writeXDL(Buffer& buf) const { write(buf, val); }

void Date::writeXDL(Buffer& buf) const { write(buf, date); }
void JulianDate::writeXDL(Buffer& buf) const { write(buf, date); }
void Timestamp::writeXDL(Buffer& buf) const { write(buf, val); }

void String8::writeXDL(Buffer& buf) const { write(buf, val); }
void String16::writeXDL(Buffer& buf) const { write(buf, val); }
void String32::writeXDL(Buffer& buf) const { write(buf, val); }
void String64::writeXDL(Buffer& buf) const { write(buf, val); }

void Struct::writeXDL(Buffer& buf) const { write(buf, *this); }
void Struct::Iterator::writeXDL(Buffer& buf) const {}
void GenericList::writeXDL(Buffer& buf) const { write(buf, *this); }
void GenericList::Iterator::writeXDL(Buffer& buf) const {}
void XDLRaw::writeXDL(Buffer& buf) const { write(buf, *this); }
void TypeDef::writeXDL(Buffer& buf) const { type->writeXDL(buf); }
void ArrayOfBytes::writeXDL(Buffer& buf) const {}

void UnImpl::writeXDL(Buffer& buf) const { throw Ex1(Errcode::UNIMPLEMENTED); }

void U8::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void U16::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void U24::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void U32::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void U64::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void U128::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void U256::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }

void I8::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void I16::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void I24::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void I32::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void I64::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void I128::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void I256::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }

void F32::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void F64::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void Bool::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }

void Date::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void JulianDate::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void Timestamp::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }

void String8::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void String16::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void String32::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void String64::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }

void Struct::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void Struct::Iterator::writeXDLMeta(Buffer& buf) const {}
void GenericList::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void GenericList::Iterator::writeXDLMeta(Buffer& buf) const {}
void XDLRaw::writeXDLMeta(Buffer& buf) const { writeMeta(buf, *this); }
void TypeDef::writeXDLMeta(Buffer& buf) const { type->writeXDLMeta(buf); }
void ArrayOfBytes::writeXDLMeta(Buffer& buf) const {}

void UnImpl::writeXDLMeta(Buffer& buf) const {
  throw Ex1(Errcode::UNIMPLEMENTED);
}
