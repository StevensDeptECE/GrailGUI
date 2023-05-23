#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "csp/SocketIO.hh"
#include "csp/csp.hh"
#include "util/List1.hh"
#include "util/datatype.hh"

class XDLRaw;

class Buffer {
 public:
  Buffer(size_t initialSize, bool writing);
  Buffer(const char filename[], size_t initialSize);
  Buffer(const char filename[], size_t initialSize, const char*);
  Buffer(const Buffer& c) = delete;
  ~Buffer() {
    if (writing && !isSockBuf) {
      flush();
    }
    delete[] preBuffer;
  }
  void operator=(const Buffer& orig) = delete;

  void attachWrite(int sockfd) {
    fd = sockfd;
    p = buffer;
    availSize = size;
  }

  void attachRead(int sockfd) {
    fd = sockfd;
    p = buffer;
    availSize = size;
    readNext();
  }

  void displayText(std::ostream& s) const;
  void displayRawRead() const;
  void displayRaw() const;
  void displayHTTPRaw();  // TODO: eliminate! die die die

  void flush() {  // TODO: this will fail if we overflow slightly
    uint32_t writeSize = (p - buffer >= size) ? size : (p - buffer);
    if (isSockBuf)
      SocketIO::send(fd, buffer, writeSize, 0);
    else {
      if (::write(fd, buffer, writeSize) < 0) throw Ex1(Errcode::FILE_WRITE);
    }
    p = buffer;
    availSize = size;
  }
  void readNext();
  // write is binary
  void write(const std::string& s);
  void write(const char* s, uint32_t len);
  void writeStructMeta(const char name[], uint32_t numMembers);

  // append is ASCII text
  void appendU8(uint8_t);
  void appendU16(uint16_t);
  void appendU32(uint32_t);
  void appendU64(uint64_t);
  void appendI8(int8_t);
  void appendI16(int16_t);
  void appendI32(int32_t);
  void appendI64(int64_t);
  void appendF32(float);
  void appendF64(double);
  void append(const char* v);
  void append(const char* v, uint32_t len);

  //    void append(float f);
  uint32_t parseU32() {
    char* temp;
    uint32_t v = strtoul(p, &temp, 10);
    p = temp;
    return v;
  }
  double parseF64() {
    char* temp;
    double v = strtod(p, &temp);
    p = temp;
    return v;
  }
  float parseF32() {
    char* temp;
    float v = strtof(p, &temp);
    p = temp;
    return v;
  }

  bool parseRegex(const std::regex& r, const char*& start, int& len);
  // if parse returns true, advance past the token
  bool parseToken(const std::string& match);
  bool getUrl(const char*& ptr, uint32_t& len);
  bool getHTTPVersion(const char*& ptr, uint32_t& len);
  bool getHost(const char*& ptr, uint32_t& len);
  void pointToStart();
  /**
     extract the next space-delimited value from the buffer
     if return true, this means ptr is pointing to the text, len = the length of
     the token (until the next space) and the current pointer advances past the
     token
  */
  bool getNextTokenWithSpace(const char*& ptr, const uint32_t& len);
  std::string readString8();
  std::string readString16();
  std::string readString32();

  /*
    Write out a data type as a single byte
   */
  void write(DataType t) { write(uint8_t(t)); }
  DataType readType() { return DataType(*p++); }

  void write(DataType t, const char* name) {
    write(t);
    write(name, strlen(name));
  }

  /**
   * write is the fast write that does not check for buffer overrun.
   * Use only when checking size of a large block
   *
   * @tparam T the tpe of the integer to write
   * @param v the value
   */
  //************ uint8_t uint16_t uint32_t uint64_t *************//
  template <typename T>
  void write(T v) {
    *(T*)p = v;
    p = p + sizeof(T);
    availSize -= sizeof(T);
  }

  /**
   * Special case for XDLRaw which will write out
   * a complete block of bytes directly without copying
   *
   */
  void write(const XDLRaw& v);

  // for writing big objects, don't copy into the buffer, write it to the socket
  // directly
  void specialWrite(const char* buf, const uint32_t len) {
    flush();
    ::write(fd, buf, len);
  }

  template <typename T>
  void writeList(List1<T>& list) {
    checkSpace(list.serializeSize());
    list.write(p);
    p += list.serializeSize();
    availSize -= list.serializeSize();
  }


  //************ uint8_t uint16_t uint32_t uint64_t array *************//
  /*
   if there is not enough space to write this data without overflowing the
   buffer overflow region, then flush.  This assumes that the object being
   written is small enough to fit into the buffer at all
   */
  void checkSpace(size_t sz) {
    if (p + sz > buffer + size + extra) {  // p>buffer+size
      flush();
    }
  }

  //************ uint8_t uint16_t uint32_t uint64_t array *************//
  /*
    The fastest way to write 32k at a time is to write each object into
                the buffer as long as it is less than the overflow size.
                Then, after writing, if you have filled the buffer, flush
                and move the remaining bytes to the beginning of the buffer and
    start over.
   */
  void fastCheckSpace(size_t sz) {
    if (p > buffer + size) {  // p>buffer+size
      uint32_t beyondEnd = p - (buffer + size);
      flush();
      memcpy(buffer, buffer + size, beyondEnd);
      p += beyondEnd;
      availSize -= beyondEnd;
    }
  }

  //*********************************//
  //************ uint8_t uint16_t uint32_t uint64_t array *************//

  template <typename T>
  void checkArraySpace(T v[], size_t n) {
    if (size < n * sizeof(T)) {
      // TODO: buffer is not big enough to copy data, write directly
    }

    // TODO: efficiency, and for big arrays if (n > ???)
    if (availSize < n * sizeof(T)) {
      flush();
    }
  }
  //*********************************//
  //************ uint8_t uint16_t uint32_t uint64_t vector *************//
  template <typename T>
  void checkVectorSpace(const std::vector<T>& v) {
    if (size < v.size() * sizeof(T)) {
      // TODO: buffer is not big enough to copy data, write directly
    }
    if (availSize < v.size() * sizeof(T)) flush();
  }

  //*********************************//
  //************ uint8_t uint16_t uint32_t uint64_t operator *************//
  template <typename T>
  Buffer& operator<<(T v) {  // there is a write in flush function
    checkSpace(sizeof(T));
    write(v);
    return *this;
  }

  /*
   TODO: For writing large arrays, it would be more efficient to flush the
   buffer, then write directly from the arrays.
*/

  int8_t _readI8() {
    int8_t temp = *(int8_t*)p;
    p += sizeof(int8_t);
    availSize -= sizeof(int8_t);
    return temp;
  }

  int16_t _readI16() {
    int16_t temp = *(int16_t*)p;
    p += sizeof(int16_t);
    availSize -= sizeof(int16_t);
    return temp;
  }

  int32_t _readI32() {
    int32_t temp = *(int32_t*)p;
    p += sizeof(int32_t);
    availSize -= sizeof(int32_t);
    return temp;
  }

  int64_t _readI64() {
    int64_t temp = *(int64_t*)p;
    p += sizeof(int64_t);
    availSize -= sizeof(int64_t);
    return temp;
  }

  int8_t readI8() {
    checkAvailableRead(sizeof(int8_t));
    return _readI8();
  }

  int16_t readI16() {
    checkAvailableRead(sizeof(int16_t));
    return _readI16();
  }

  int32_t readI32() {
    checkAvailableRead(sizeof(int32_t));
    return _readI32();
  }

  int64_t readI64() {
    checkAvailableRead(sizeof(int64_t));
    return _readI64();
  }

  uint8_t _readU8() {
    uint8_t temp = *(uint8_t*)p;
    p += sizeof(uint8_t);
    availSize -= sizeof(uint8_t);
    return temp;
  }

  uint16_t _readU16() {
    uint16_t temp = *(uint16_t*)p;
    p += sizeof(uint16_t);
    availSize -= sizeof(uint16_t);
    return temp;
  }

  uint32_t _readU32() {
    uint32_t temp = *(uint32_t*)p;
    p += sizeof(uint32_t);
    availSize -= sizeof(uint32_t);
    return temp;
  }

  uint64_t _readU64() {
    uint64_t temp = *(uint64_t*)p;
    p += sizeof(uint64_t);
    availSize -= sizeof(uint64_t);
    return temp;
  }

  float _readF32() {
    float temp = *(float*)p;
    p += sizeof(float);
    availSize -= sizeof(float);
    return temp;
  }

  double _readF64() {
    double temp = *(double*)p;
    p += sizeof(double);
    availSize -= sizeof(double);
    return temp;
  }

  uint8_t readU8() {
    checkAvailableRead(sizeof(uint8_t));
    return _readU8();
  }

  uint16_t readU16() {
    checkAvailableRead(sizeof(uint16_t));
    return _readU16();
  }

  uint32_t readU32() {
    checkAvailableRead(sizeof(uint32_t));
    return _readU32();
  }

  uint64_t readU64() {
    checkAvailableRead(sizeof(uint64_t));
    return _readU64();
  }

  float readF32() {
    checkAvailableRead(sizeof(float));
    return _readF32();
  }

  double readF64() {
    checkAvailableRead(sizeof(double));
    return _readF64();
  }

  template <typename T>
  T _read() {
    T temp = *(T*)p;
    p += sizeof(T);
    availSize -= sizeof(T);
    return temp;
  }

#if 0
  template <typename T>
  T _readString() {
    char length = *p;
    char word[length + 1];
    memcpy(word, p + 1, length);
    word[length] = 0;
    return word;
  }

  template <typename T>
  T read() {
    // cout << (uint64_t)p << endl;
    checkAvailableRead(sizeof(T));
    return _read<T>();
  }

  template <typename T>
  T readString() {
    return _readString<T>();
  }

  template <typename T>
  List1<T> readList() {
    uint32_t len = read<uint32_t>();
    List1<T> temp(len);
    for (uint32_t i = 0; i < len; i++) {
      T val = read<T>();
      temp.add(val);
      // TODO: For some reason code does not work without this print statement.
      // More specifically the endl.
      // TODO: It appears that this is no longer necessary after the switch to
      // vector
      /*std::cout.setstate(std::ios::failbit);
      //cout<<temp.getData(i)<<endl;
      cout<<""<<endl;
      std::cout.clear();*/
    }
    return temp;
  }
#endif

 private:
  bool writing;
  bool isSockBuf = false;
  size_t size;
  const size_t extra = 128;
  char* preBuffer;
  char* buffer;       // pointer to the buffer
  int32_t availSize;  // how much space is left in the buffer
  char* p;            // cursor to current byte for reading/writing
  int fd;  // file descriptor for file backing this buffer (read or write)
  uint32_t blockSize;  // Max block size for output
  void checkAvailableRead(size_t sz) {
    if (availSize < sz) {
      size_t overflowSize = availSize;
      memcpy(buffer - overflowSize, p, overflowSize);
      readNext();
      availSize += overflowSize;
      p = buffer - overflowSize;
    }
  }

 public:
  void checkAvailableWrite() {
    if (p > buffer + size) {
      uint32_t overflow = p - (buffer + size);
      flush();
      memcpy(p, p + size, overflow);
      p += overflow;
      availSize -= overflow;
    }
  }

 private:
  void checkAvailableWrite(const char* ptr, uint32_t len) {
    if (p + len > buffer + size) {
      memcpy(p, ptr, availSize);
      uint32_t remaining = len - availSize;
      flush();
      // TODO: Check if the string is too big!
      memcpy(p, ptr + size, remaining);
      p += remaining;
      availSize -= remaining;
      return;
    }
    // TODO: check case where string is bigger than buffer
    if (len > size) {
      flush();
      // TODO: Do something completely different
      ::write(fd, ptr, len);
      return;
    }
    memcpy(p, ptr, len);
    p += len;
    availSize -= len;
  }
};
