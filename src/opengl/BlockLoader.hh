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
  // std::unique_ptr<uint64_t> mem;
  uint64_t* mem;  // hating unique pointers right now
  uint64_t size;
  struct GeneralHeader {
    uint32_t magic;         // magic number for all block loaders
    uint32_t type : 16;     // type of block loader
    uint32_t version : 16;  // version
  };
  struct SecurityHeaderV0 {
    uint64_t yoho;  // TODO: put something in
  };
#if 0
	struct SecurityHeaderV1 {
		uint8_t hash[32];  // SHA256 hash signed by author
		uint8_t hash2[32]; // SHA256 hash of alternate region in data signed by author
		uint8_t id[32]; // id of author
	};
#endif

  GeneralHeader* generalHeader;
  SecurityHeaderV0* securityHeader;
  enum class Type { gismap, hashmap };

 protected:
  /*
    TODO: Find better way than uninitialized parent?
    problem: We need to allocate memory in base class to share
    memory allocation and common headers among all BlockLoaders.
    But we don't know how much memory we need until we open the file
    read all the information from ESRI and create.
    In any file format, there will be this problem. There must be a
    temporary object structure while you read in and figure out how big
    the data is. Then, once the data is known, you can read in a single
    contiguous block of memory, fill it and save that out to disk for
    rapid loading later.

    One way to eliminate this problem might be to allocate the header and
    what for us is the segment information for the map with lots of
    extra space at the end. We make the list as big as it needs to be and
    just keep an extra data structure with the points. Then, when done
    growing define a pointer to the points at the end and copy in.

    The uninitialized protected constructor is only for use by children
    who first figure out how big, then call init() to initialize their
    parent. The only alternative would be to call the parent constructor
    passing in the information (type and version is easy, number of bytes
    requires knowing everything before call) This would require returning
    a structure with those 3 items. Should probably just do that.
  */
  BlockLoader() {}
  struct Info {
    uint64_t bytes;
    Type t;
    uint32_t version;
  };
  BlockLoader(const Info& info);

 public:
  BlockLoader(const char filename[]);
  ~BlockLoader() { delete[] mem; }
  BlockLoader(const BlockLoader& orig) = delete;
  BlockLoader& operator=(const BlockLoader& orig) = delete;

  void init(uint64_t* mem, uint64_t size);
  void init(uint64_t bytes, Type t, uint32_t version);
  // Fast load a blockfile
  void readBlockFile(const char filename[]);
  // get the size of the SecurityHeader for any BlockLoader
  uint32_t getAuthHeaderSize() const { return sizeof(SecurityHeaderV0); }
  uint32_t getHeaderSize() const {
    return sizeof(GeneralHeader) + sizeof(SecurityHeaderV0);
  }
  bool authenticate() const;
  BlockLoader(uint64_t bytes, Type t, uint32_t version)
      : mem(new uint64_t[(getHeaderSize() + (bytes + 7) / 8)]) {
    // std::make_unique<uint64_t[]>(getHeaderSize() + (bytes + 7) / 8)) {
    generalHeader = (GeneralHeader*)mem;  // header is the first chunk of bytes
    generalHeader->magic = ((((('!' << 8) + 'B') << 8) + 'L') << 8) +
                           'd';  // magic number for all block loaders
    generalHeader->type = uint32_t(t);
    generalHeader->version = version;
    securityHeader =
        (SecurityHeaderV0*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
  }
};
