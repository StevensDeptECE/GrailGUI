#include "csp/Page.hh"
using namespace std;

void Page::readOffsetVector(ifstream& infile) {
  while (!infile.eof()) {
    uint32_t offset;
    uint32_t type;
    infile >> offset;
    infile >> type;
    insertions.emplace_back(Insertion(offset, DataType(type)));
    size += dynamicSize(DataType(type));
  }
}

Page::Page(const string& inputFile) {
  ifstream infile;
  insertions = vector<Insertion>();
  infile.open(inputFile);
  if (!infile) {
    throw "Cannot open file";
  }
  infile >> size;
  staticContent.resize(size);
  infile.read(&staticContent[0u], size);
  staticSize = size;
  infile.seekg(8, ios::cur);
  if (!infile.eof()) readOffsetVector(infile);
  infile.close();
}

Page::Page(const string& staticFile, const string& insertFile)
    : staticContent(staticFile) {
  ifstream infile;
  infile.open(staticFile);
  if (!infile) {
    throw "Cannot open file";
  }
  infile.seekg(0, infile.end);
  size = infile.tellg();
  infile.close();

  insertions = vector<Insertion>();
  infile.open(insertFile);
  while (!infile.eof()) {
    uint32_t offset;
    int type;
    infile >> offset;
    infile >> type;
    insertions.emplace_back(Insertion(offset, DataType(type)));
    size += dynamicSize(DataType(type));
  }
}

ostream& operator<<(ostream& s, const Page& p) {
  s << p.getStaticSize() << '\n';
  s << p.getStaticContent() << '\n';
  const vector<Insertion>& inserts = p.getInsertions();
  s << inserts.size() << '\n';
  for (auto i : inserts) s << i.offset << ' ' << DataTypeNames[i.d] << '\n';
  return s;
}

uint32_t Page::dynamicSize(DataType d) {
  // TODO: How to handle variable length data types (strings)
  // TODO: Can the DataType enum be used to hold the sizes of the datatypes?
  // For now just use max possible values for all data - Just use 256 bytes for
  // a string for now
  // TODO: Figure out what to do if we exceed max value
  switch (d) {
    case UINT1:
      return 3;
    case UINT2:
      return 5;
    case UINT4:
      return 10;
    case UINT8:
      return 20;
    case INT1:
      return 3;
    case INT2:
      return 5;
    case INT4:
      return 10;
    case STRING1:
      return 255;
    case LIST2:
      return 1500;
    case FLOAT:
      return 8;
    case DOUBLE:
      return 16;
    default:
      return 0;
  }
}
