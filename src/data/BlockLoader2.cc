
#include "data/BlockLoader2.hh"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fmt/core.h"
#include "util/Ex.hh"
#include "util/PlatFlags.hh"

BlockLoader::BlockLoader(uint64_t bytes, Type t, uint16_t version)
    : mem((uint64_t*)(malloc((getHeaderSize() + ((bytes + 7) & ~7ULL))))), size(bytes) {
  generalHeader = (GeneralHeader*)mem;  // header is the first chunk of bytes
  generalHeader->magic = ((((('!' << 8) + 'B') << 8) + 'L') << 8) +
                         'd';  // magic number for all block loaders
  generalHeader->type = uint32_t(t);
  generalHeader->version = version;
  generalHeader->author_id = 0;  // author id not defined until registered
  generalHeader->doc_id =
      0;  // document id not defined without getting a unique id from server
}

BlockLoader::BlockLoader(const char filename[], uint64_t fileSize, uint64_t memSize) {
  int fh = open(filename, O_RDONLY | O_BINARY);
  if (fh < 0) throw Ex2(Errcode::FILE_READ, "Can't open file");
  struct stat s;
  fstat(fh, &s); // get the size of the file
  if (memSize != 0)
    size = ((memSize + 7)/8)* 8; // if size is specified manually, round up to next multiple of 8 (byte alignment)
  else
    size = ((s.st_size + 7)/8)* 8; // if file size is not multiple of 8, round up to 8 for byte alignment
  mem = (uint64_t*)malloc(size);
  int bytesRead = read(fh, (char*)mem, fileSize); // read the entire file into RAM
//  fmt::print("{:d} bytes read, size is {:d}\n", bytesRead, fileSize);
  if (bytesRead != fileSize)
    throw Ex2(Errcode::FILE_READ, "Could not read entire file");
  generalHeader = (GeneralHeader*)mem;  // header is the first chunk of bytes
  close(fh);
}
#if 0
BlockLoader BlockLoader::readFile(const char filename[], uint64_t memSize = 0) {
  int fh = open(filename, O_RDONLY | O_BINARY);
  if (fh < 0) throw Ex2(Errcode::FILE_READ, "Can't open file");
  struct stat s;
  fstat(fh, &s);
  if (memSize != 0)
    return BlockLoader(fh, s.st_size, memSize); // for extra memory
  else
    return BlockLoader(fh, s.st_size, s.st_size); // fileSize = memSize
}
#endif

/*
** TODO: Attempted security to make hash collisions much harder (unimplemented)
*/
uint64_t BlockLoader::crc64(uint32_t start, uint32_t stride) const { return 0; }
/*
** To prevent a collision attack where a document is designed to have the same
*hash but be different
** Hash in multiple ways, including some the attacker does not know in advance.
*This would
** make it much harder to forge a priori
*/
void BlockLoader::hashThisDocument() const {
  // TODO: multiple CRC can be computed more efficiently together
  uint64_t crc1 = crc64(0, 1);  // CRC of entire file
  uint64_t crc2 = crc64(0, 2);  // CRC of even bytes
  uint64_t crc3 = crc64(1, 2);  // CRC of odd bytes
  uint64_t crc4 = crc64(0, 3);  // CRC of every 3rd byte (mod 0)
  uint64_t crc5 = crc64(1, 3);  // CRC of every 3rd byte (mod 1)
  // sha256 hash entire file
  // sha256 hash even bytes
  // sha256 hash odd bytes
  // sha256 hash even 1k blocks
  // sha256 hash odd 1k blocks
  //
  // add author-defined parameter permuting file and hashing. Anyone designing a
  // document that collides using the above known crc and sha hashes would also
  // have to match the unknown This information is stored encrypted and in the
  // event of a dispute, the author can prove the hash was valid.
}

void BlockLoader::registerDocument(uint64_t author_id) const {
  // connect to server
  // digitally authenticate user (will require asymmetric key)
  // hash this document in multiple ways
  // because there is a known collision problem with attacks increasingly
  // successful as time goes on conservatively compute multiple hashes to make
  // it harder to construct a document that meets all criteria perhaps even a
  // secret criteria known only to the author details in the prototype
  // hashThisDocument() method
  hashThisDocument();
  // get document id from server that is unique per author (each author can have
  // document id 0, 1, etc) store the hashes on the server so the document can
  // be identified
  generalHeader->author_id = author_id;  // record the author in the document
  generalHeader->doc_id = 0;  // TODO: create unique document id for this author
}

bool BlockLoader::authenticateDocument() const {
  return false;  // TODO: authentication not implemented yet
}

void BlockLoader::writeFile(const char filename[], uint64_t fileSize) {
  int fh = open(filename, O_BINARY | O_WRONLY | O_TRUNC | O_CREAT, 0644);
  int bytesWritten = write(fh, (char*)mem, fileSize);
  if (bytesWritten < 0) {
    throw Ex2(Errcode::FILE_READ, strerror(errno));
  }
//  fmt::print("{:d} bytes written\n", bytesWritten);
  close(fh);
}

void BlockLoader::writeFile(const char filename[]) {
  writeFile(filename, size);
}