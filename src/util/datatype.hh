#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "util/HashMap.hh"

/**
 * @brief An enumerated class of XDL types that can be transferred
 *
 */
enum class DataType {
  U8,    // unsigned int 0..255
  U16,   // unsigned int 0..65535
  U24,   // unsigned int 0..16*1024*1024
  U32,   // unsigned int 0..4200000000
  U64,   // 64 bits
  U128,  // 128 bits
  U256,  // 256 bits
  I8,    // unsigned int 0..255
  I16,   // unsigned int 0..65535
  I24,   // unsigned int 0..16*1024*1024
  I32,   // unsigned int 0..4200000000
  I64,   // 64 bits
  I128,  // 128 bits
  I256,  // 256 bits
  F32,
  F64,
  BOOL,       // true/false, one byte but for arrays should use 1 bit
  DATE,       // 32 bit date relative to J2000 (integer)
  JULDATE,    // scientific date based on double relative to J2000
  TIMESTAMP,  // typical unix timestamp
  STRING8,    // string with 1 byte length 0..255 each character one byte
  STRING16,   // string with 2 byte length 0..65535 each character one byte
  STRING32,   // string with 4 byte length 0..4.2B each character 1 byte
  STRING64,   // string with 8 byte length 4.2B^2 (huge!)
  UTF8_8,     // international string with 1 or 2 bytes, 1 byte size
  UTF8_16,    // international string with 1 or 2 bytes, 2 byte size
  UTF8_32,    // international string with 1 or 2 bytes, 4 byte size
  UTF8_64,    // international string with 1 or 2 bytes, 8 byte size
  UTF16_8,    // international string with 2 bytes per char, 1 byte size
  UTF16_16,   // international string with 2 bytes per char, 2 byte size
  UTF16_32,   // international string with 2 bytes per char, 4 byte size
  UTF16_64,   // international string with 2 bytes per char, 8 byte size
  REGEX,

  LIST8,     // LIST with length encoded as 1 unsigned byte 0..255
  LIST16,    // list with length encoded as 2 bytes 0 ... 65535
  LIST32,    // list with length encoded as 4 bytes 0 ... 4.2B
  LIST64,    // list with length encoded as 8 bytes 0 ... 2^64-1
  STRUCT8,   // define new integer record identifier (number).  The number of
             // records is 0..255
  STRUCT16,  // same as record1, but can handle up to 0..65535 fields
  STRUCT32,  // 4 byte number of components, HUGE and probably useless, but here
             // just in case

  DYNAMICLIST1,  // list sent from dynamic source.  The number is how many sent
                 // this time, with a boolean to tell us when it is over
  DYNAMICLIST2,  // same but with 2byte count.  This is useful for databases
                 // where it is much faster to start getting data and figure out
                 // the total later
  FUNC1,  // defines new id of function with up to 256 instructions (no zero
          // because a zero-length function is useless) When executed can run
          // sequence of commands, length = number of contained commands
  FUNC2,  // same as FUNC1 with up to 65536 steps
  FUNCPARAM1,  // takes number of parameters (up to 256) and number of steps (up
               // to 256)
  FUNCPARAM2,  // takes number of parameters (up to 65536) and number of steps
               // (up to 65536)
  LOOP1,       // repeat n times count 0-255
  LOOP2,       // repeat n times count 0-65535
  LOOP4,       // repeat n times count 0-4.2B
  BITVEC8,     // bit vector with length 1 byte
  BITVEC16,    // bit vector with length 2 bytes
  BITVEC32,    // bit vector with length 4 bytes
  BITVEC64,    // bit vector with length 8 bytes
  BV32,        // 32 bits stored in a 4-byte integer
  BV64,        // 64 bits stored in a 8-byte integer
  BITFIELDS,   // 4-byte size, 1-byte bit size (for storing small numbers packed
               // bitfields)
  ARITHMETIC,  // arithmetic encoding of numbers specify range and size (each 4
               // bytes)
  JPEG,        // introducer to JPEG image embedded in stream
  JPEGLIST,  // 4 byte number of jpegs, followed by a single header, followed by
             // multiple files of same size
  BLOB32,    // 4 byte length block of binary data (opaque to us)
  BLOB64,    // 8 byte length block of binary data
  BIGINT,
  TYPEDEF,
  UNIMPL,
  ENUM_SIZE
};

extern const char* DataTypeNames[];
extern HashMap<DataType> mapnames;
extern void loadmap();

/*

Examples of packet data:  All numbers shown are bytes, NOT ascii text numbers

Example1: fixed data with one 64 bit unsigned integer, and one double precision
number. Server sends 8 + 8 bytes, the client knows from context exactly what is
expected for this page. 1234567890 123.456

Example2: A table with 3 fixed columns: first name, last name, phone.  The
client knows the data type of each column, so it is not specified, but since
strings are variable, the lengths are encoded.  This example has types:
string1,string1,uint8
The length of the list is not known, so it is transmitted
LIST1 2
3Dov6Kruger2125551212
8Branimir6Kruger2123331215

Example3: A query goes to the server and the columns are not known in advance,
so the server encodes the metadata RECORD1 5 string1 string1 string1 uint1
  float4

list1 1
  3Dov 6Kruger 22Dov.Kruger@stevens.edu 2015551212 20.4512121

Note: functions are only used in binary HTML representation.  Typically you
would never need a function to send between machines, and you can always write
the code longhand, without a function. Therefore, functions are an optimization
to more compactly encode binary HTML when there are repetitive features.
Eventually, functions may become a way to create higher-level entities that can
create reusable packages. Probably objects will be useful as well.

CSP is not a full programming language by design, but if we can build in ways to
safely process data locally on the client while remaining provably safe, we can
do that.

FUNCPARAM1 (uint4 c1, uint4 c2, uint4 c3)
 <tr class="foo"><td><%= c1 %></td><td><%= c2 %></td><td><%= c3 %></td></tr>

FUNCPARAM1
  3 7  // 3 parameters, contents 7 entities
  c1 c2 c3  // 24 bytes
  6 2 1  // length 6 contents TR class foo is 1 (3 bytes)
    2 3 0  // contents length=2 TD class unspecified (default) (3 bytes)
    PARAM 0 // c1
    2 3 0  // contents length = 2 TD default class  (3 bytes)
    PARAM 1
    2 3 0
    PARAM 2

 */

/*
class List1 {
private:
        uint8_t size;
public:
        List1(uint8_t size) : size(size) {}
};
*/

class List2 {
 private:
  uint16_t size;

 public:
  List2(uint16_t size) : size(size) {}
};

class Record {
 private:
  std::vector<DataType> fieldTypes;

 public:
  Record() {}
  void add(DataType t) { fieldTypes.push_back(t); }
  friend std::ostream& operator<<(std::ostream& s, const Record& r) {
    DataType t = r.fieldTypes.size() < 256
                     ? DataType::STRUCT8
                     : (r.fieldTypes.size() < 65536 ? DataType::STRUCT16
                                                    : DataType::STRUCT32);
    s << DataTypeNames[uint32_t(t)];
    for (auto x : r.fieldTypes) {
      s << DataTypeNames[uint32_t(x)] << ' ';
    }
    return s;
  }
};
