#pragma once
#include <memory>

/*
  A BlockLoader is a class of file format designed to be loaded in a
  single read Ultra-efficient load times, low CPU utilization with a
  header that supports a common architecture for digital signature and
  authentication across all BlockLoader Types.
*/
class BlockLoader {
 public:
  uint64_t* mem;  // In C++20 we can have an unzeroed unique_ptr
  uint64_t size;
  char* align(const char* p) { return (char*)((uint64_t)p + 7) & ~7ULL; }

  enum class Type { gismap, hashmap, compressed_trie, gapminder, financial_db };
  enum CompressionType { none, lzma, bzip2 };
  struct GeneralHeader {
    uint32_t magic;    // magic number for all block loaders
    uint16_t type;     // type of block loader
    uint16_t version;  // version
    // TODO: copyright, copyleft, no protection? (full reference in server where
    // authenticated?)
    uint64_t copyright : 1;       // true if copyright is asserted
    uint64_t securityHeader : 1;  // if true, there is a security header
    // the number of hashmaps in the optional HashmapHeader
    // used for named entities
    uint64_t numComponents : 16;
    uint64_t compressionType : 8;  // lzma, bzip2, etc.
    GeneralHeader(uint16_t type, uint16_t version, bool copyright = false,
                  bool securityHeader = false, uint8_t numComponents = 0,
                  uint8_t compressionType = 0)
        : magic(0x644C4221),  // "!Bld" magic number for block loaders
          type(type),
          version(version),
          copyright(copyright),
          securityHeader(securityHeader),
          numComponents(numComponents),
          compressionType(compressionType)
  };
  struct SecurityHeaderV1 {
    uint8_t hash[32];  // SHA256 hash signed by author
    // SHA256 hash of alternate region in data signed by author
    uint8_t hash2[32];
    uint8_t id[32];  // id of author
  };

  /*
    optional header for a hash map of strings

  */
  struct HashMapHeader {
    uint32_t symbolSize;  // total number of bytes used for symbols
    uint32_t nodeSize;    // total number of nodes used
    uint32_t tableSize;   // number of bins, must be power of 2
  };

  GeneralHeader* generalHeader;
  SecurityHeaderV1* securityHeader;

 protected:
  struct Info {
    uint64_t bytes;
    Type type;
    uint32_t version;
  };
  BlockLoader(const Info& info);
  void BlockLoader::init(uint64_t* mem, uint64_t size) {
    this->mem = mem;
    this->size = size;
    this->generalHeader = (GeneralHeader*)mem;
    this->securityHeader =
        (SecurityHeaderV1*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
  }

 public:
  BlockLoader(const char filename[]);
  ~BlockLoader() { delete[] mem; }
  BlockLoader(const BlockLoader& orig) = delete;
  BlockLoader& operator=(const BlockLoader& orig) = delete;

  /*
   * build a new blockloader in memory using all the information needed to build
   * it the number of bytes the child needs, the type of loader, and the version
   */
  BlockLoader(Info inf)
      : mem(new uint64_t[(getHeaderSize() + (inf.bytes + 7) / 8)]) {
    generalHeader = (GeneralHeader*)mem;  // header is the first chunk of bytes
    // "!Bld" magic number for block loaders
    generalHeader->magic = 0x644C4221;
    generalHeader->type = uint16_t(inf.type);
    generalHeader->version = inf.version;
    securityHeader =
        (SecurityHeaderV1*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
  }
  // get the size of the SecurityHeader for any BlockLoader
  uint32_t getAuthHeaderSize() const { return sizeof(SecurityHeaderV1); }
  uint32_t getHeaderSize() const {
    return sizeof(GeneralHeader) + sizeof(SecurityHeaderV1);
  }
  bool authenticate() const;
  void save(const char filename[]) const;
};
