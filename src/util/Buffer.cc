#include "util/Buffer.hh"

#include <csp/HTTPRequest.hh>

#include "csp/SocketIO.hh"
#include "csp/csp.hh"
#include "xdl/std.hh"
using namespace std;
// const string HTTPRequest::GET = "GET";

/*
  open a buffer on a socket for writing
*/
Buffer::Buffer(size_t initialSize, bool writing)
    : writing(writing), size(initialSize) {
  availSize = writing ? size : 0;
  preBuffer = new char[size + extra * 2];
  buffer = extra + preBuffer;
  p = buffer;
  memset(preBuffer, '\0', size + extra * 2);
  fd = -1;
  isSockBuf = true;
}

/*
  open a buffer on a file for writing
*/
Buffer::Buffer(const char filename[], size_t initialSize)
    : Buffer(initialSize, true) {
  fd = creat(filename, 0664);
  if (fd < 0) throw Ex1(Errcode::PERMISSION_DENIED);
  isSockBuf = false;
}

/*
  Open a buffer for reading on an existing file (third bogus parameter
  disambiguates)
*/
Buffer::Buffer(const char filename[], size_t initialSize, const char*)
    : Buffer(initialSize, false) {
  fd = open(filename, O_RDONLY | O_BINARY);
  if (fd < 0) throw Ex1(Errcode::PERMISSION_DENIED);
  readNext();
  writing = false;
  isSockBuf = false;
}

void Buffer::readNext() {
  int32_t bytesRead = SocketIO::recv(fd, buffer, size, 0);
  if (bytesRead > 0) availSize -= bytesRead;
  // TODO: do we set p????
  // read really shouldn't return negative but it is, at least on windows...
  // check why This is occurring when there are no bytes to read -- possible
  // they're returning -1 when most return 0?
}

// TODO: This string does nto check if there is available buffer for it!! BUG
// TODO: For now, we are only using String8. We have to encode what kind of
// string is coming if we support variable sizes
void Buffer::write(const string& s) {
  // if(s.length() < 256) {
  *(uint8_t*)p++ = s.length();
#if 0
    }else if (s.length() < 65536){
        *(uint16_t*)p++ = s.length();
        availSize -= sizeof(uint16_t);
    } else {
        *(uint32_t*)p++ = s.length();
        availSize -= sizeof(uint32_t);
    }
#endif
  for (uint8_t i = 0; i < s.length(); i++) {
    *p++ = s[i];
  }
  availSize -= (s.length() + 1);
}

void Buffer::write(const char* s, uint32_t len) {
  // TODO: assuming that string < 256 bytes
  if (len > 255) throw Ex1(Errcode::STRING_TOO_LONG);
  *p++ = len;
  checkAvailableWrite();
  checkAvailableWrite(s, len);
}

void Buffer::write(const XDLRaw& v) {
  if (p != buffer) flush();
  ::write(fd, v.data, v.len);
}

#if 0
void Buffer::write(const char s[], uint32_t len) {
  if (len >= 256) throw Ex1(Errcode::STRING_TOO_LONG);
  *(uint8_t*)p++ = len;
  for (uint8_t i = 0; i < len; i++) {
    *p++ = s[i];
  }
}
#endif

void Buffer::writeStructMeta(const char name[], uint32_t numMembers) {
  if (numMembers <= UINT8_MAX) {
    write(DataType::STRUCT8);
    write(name, strlen(name));
    write((uint8_t)numMembers);
  } else if (numMembers <= UINT16_MAX) {
    write(DataType::STRUCT16);
    write(name, strlen(name));
    write((uint16_t)numMembers);
  } else if (numMembers <= UINT32_MAX) {
    write(DataType::STRUCT32);
    write(name, strlen(name));
    write(numMembers);
  }
}

string Buffer::readString8() {
  checkAvailableRead(sizeof(uint8_t));
  uint8_t len = *(uint8_t*)p;
  p += sizeof(uint8_t);
  availSize -= sizeof(uint8_t);
  string s;
  s.reserve(len);
  checkAvailableRead(len);
  for (size_t i = 0; i < len; i++) {
    s += *(char*)p;
    p += sizeof(char);
  }
  availSize -= len;
  return s;
}

string Buffer::readString16() {
  checkAvailableRead(sizeof(uint16_t));
  uint16_t len = *(uint16_t*)p;
  p += sizeof(uint16_t);
  availSize -= sizeof(uint16_t);
  string s;
  s.reserve(len);
  checkAvailableRead(len);
  for (size_t i = 0; i < len; i++) {
    s += *(char*)p;
    p += sizeof(char);
  }
  availSize -= len;
  return s;
}

string Buffer::readString32() {
  checkAvailableRead(sizeof(uint32_t));
  uint32_t len = *(uint32_t*)p;
  p += sizeof(uint32_t);
  availSize -= sizeof(uint32_t);
  string s;
  s.reserve(len);
  checkAvailableRead(len);
  for (size_t i = 0; i < len; i++) {
    s += *(char*)p;
    p += sizeof(char);
  }
  availSize -= len;
  return s;
}

void Buffer::appendU8(uint8_t v) {  // maximum size 255
  if (v >= 100) {
    uint32_t d = v / 100;
    *p++ = d + '0';
    availSize--;
    v = v - d * 100;
  }
  if (v >= 10) {
    uint32_t d = v / 10;
    *p++ = d + '0';
    availSize--;
    v = v - d * 10;
  }
  *p++ = v + '0';
  availSize--;
  checkAvailableWrite();
}

// TODO: These appends are slow because they count bytes, should instead
// insert small values into the buffer and if they overflow, copy back to the
// beginning after flushing.

void Buffer::appendU16(uint16_t v) {
  uint32_t len = sprintf(p, "%hu", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendU32(uint32_t v) {
  uint32_t len = sprintf(p, "%u", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendU64(uint64_t v) {
  uint32_t len = sprintf(p, "%lu", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendI8(int8_t v) {  // -128..127
  int32_t tmp = v;
  if (v < 0) {
    *p++ = '-';
    tmp = -tmp;
    availSize--;
  }
  // guaranteed tmp is positive
  if (tmp >= 100) {
    *p++ = '1';
    availSize--;
    tmp -= 100;
  }
  if (tmp >= 10) {
    uint32_t d = tmp / 10;
    *p++ = d + '0';
    availSize--;
    tmp -= d * 10;
  }
  *p++ = tmp + '0';
  availSize--;
  checkAvailableWrite();
}

// TODO: These appends are slow because they count bytes, should instead
// insert small values into the buffer and if they overflow, copy back to the
// beginning after flushing.

void Buffer::appendI16(int16_t v) {
  uint32_t len = sprintf(p, "%hd", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendI32(int32_t v) {
  uint32_t len = sprintf(p, "%d", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendI64(int64_t v) {
  uint32_t len = sprintf(p, "%ld", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendF32(float v) {  // 1.2 1   -1.234567e+38
  uint32_t len = sprintf(p, "%f", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::appendF64(double v) {  // 1.2 1   -1.23456789012345e+138
  uint32_t len = sprintf(p, "%lf", v);
  p += len;
  availSize -= len;
  checkAvailableWrite();
}

void Buffer::append(const char* v) {
  while (true) {
    while (*v != '\0' && availSize > 0) {  // ASCII NULL
      *p++ = *v++;
      availSize--;
    }
    if (*v == '\0') return;
    if (availSize == 0) {
      flush();
    }
  }
}

void Buffer::append(const char* v, uint32_t len) {
  if (len > availSize) {
    memcpy(p, v, availSize);
    v += availSize;
    len -= availSize;
    flush();
    if (len > availSize) {
      // TODO: write huge strings in place
    }
    return;
  }
  memcpy(p, v, len);
  p += len;
  availSize -= len;
}

void Buffer::displayText(ostream& s) const { s << buffer << '\n'; }

void Buffer::displayRawRead() const {
  uint32_t used = size - availSize;  // the number of bytes used
  for (uint32_t i = 0; i < used; ++i) cout << uint32_t(p[i]) << ' ';
  cout << '\n';
}

void Buffer::displayRaw() const {
  uint32_t used = size - availSize;  // the number of bytes used
  cout << hex << "\n";
  for (uint32_t i = 0; i < used; ++i) cout << uint32_t(p[i]) << ' ';
  cout << '\n';
  for (uint32_t i = 0; i < used; ++i) {
    if (buffer[i] >= ' ' && buffer[i] <= 127)
      cout << buffer[i];
    else
      cout << ' ';
  }
  cout << '\n';
}

// TODO: This must die. A display function for http that modifies the buffer?
void Buffer::displayHTTPRaw() {
  while (true) {
    if (availSize == 0) {
      cout << buffer;
      p = buffer;
      availSize = size;
      readNext();
    } else {
      cout << string(buffer, availSize);
      break;
    }
  }
}

bool Buffer::parseRegex(const regex& r, const char*& start, int& len) {
  return true;
}

bool Buffer::parseToken(const string& match) {
  const char* m = match.c_str();
  int hasToken = strncmp(p, m, match.length());
  if (hasToken == 0)
    for (uint32_t i = 0; i < match.length(); i++) {
      p++;
    }
  return hasToken == 0;  // true if match
}

bool Buffer::getUrl(const char*& ptr, uint32_t& len) {
  while (*p == ' ' || *p == '\r' || *p == '\n') {
    if (p >= buffer + size) return false;
    p++;
  }
  ptr = p;
  len = 0;
  while (*p != ' ') {
    p++;
    len++;
  }
  // allocate space for member variable "url" and assign it by the "requested
  // url"
  char* temp = new char;
  strncpy(temp, ptr, len);
  ptr = temp;
  return true;  // for now, assume it always works
}

bool Buffer::getHTTPVersion(const char*& ptr, uint32_t& len) {
  while (*p == ' ' || *p == '\r' || *p == '\n') {
    if (p >= buffer + size) return false;
    p++;
  }
  const char* m = "HTTP/";
  int hasToken = strncmp(p, m, strlen(m));

  if (hasToken == 0) {
    for (uint32_t i = 0; i < strlen(m); i++) {
      p++;
    }
  } else
    return false;
  ptr = p;
  len = 0;
  while (*p != '\n') {
    p++;
    len++;
  }
  char* temp = new char;
  strncpy(temp, ptr, len);
  ptr = temp;
  return true;  // for now, assume it always works
}

bool Buffer::getHost(const char*& ptr, uint32_t& len) {
  while (*p == ' ' || *p == '\r' || *p == '\n') {
    if (p >= buffer + size) return false;
    p++;
  }

  const char* m = "Host: ";
  int hasToken = strncmp(p, m, strlen(m));

  if (hasToken == 0) {
    for (uint32_t i = 0; i < strlen(m); i++) {
      p++;
    }
  } else
    return false;

  len = 0;
  ptr = p;
  while (*p != '\n') {
    p++;
    len++;
  }
  char* temp = new char;
  strncpy(temp, ptr, len);
  ptr = temp;
  return true;
}

void Buffer::pointToStart() { p = buffer; }

bool Buffer::getNextTokenWithSpace(const char*& ptr, const uint32_t& len) {
  while (isspace(*p) == 0) {
    p++;
  }
  int matched = strncmp(p, ptr, len);
  *(uint32_t*)p++ = len;
  return matched == 0;
}
