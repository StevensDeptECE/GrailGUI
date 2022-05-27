#pragma once
#include <iostream>
#include <memory>

/*
  A BlockLoader is a class of file format designed to be loaded in a
  single read Ultra-efficient load times, low CPU utilization with a
  header that supports a common architecture for digital signature and
  authentication across all BlockLoader Types.
*/
class BlockLoader {
 public:
  enum class Type {
    gismap,   // a map storing GIS shapes, akin to ESRI Shapefile but much more
              // efficient
    namemap,  // a hashmap with string keys, values are byte chunks defined by
              // user
    gapminder,  // a binary database of floating point variables, originally
                // from gapminder this one could store any time series data by
                // year
    i32map  // a hashmap with 32-bit int keys, values are byte chunks defined by
            // user
  };

  // std::unique_ptr<uint64_t> mem;
  uint64_t* mem;
  uint64_t size;
  struct GeneralHeader {
    uint32_t magic;         // magic number for all block loaders
    uint16_t type;          // type of block loader
    uint16_t version;       // version
    uint64_t author_id;     // key to find author id on server
    uint64_t doc_id;        // unique id of document by author
    uint16_t num_sections;  // the number of sections, each requiring a header
    uint16_t header_size;   // number of 64-bit words used for all headers
    static constexpr uint32_t bh = 0x644C4221;  // !BLd
    GeneralHeader(Type type, uint16_t version)
        : magic(bh), type(uint16_t(type)), version(version) {}
  };

  GeneralHeader* generalHeader;

 protected:
  /*
    TODO: Find better way than uninitialized parent?
    problem: We need to allocate memory in base class to share
    memory allocation and common headers among all BlockLoaders.
    But we don't know how much memory we need until we open the file
    read all the information from a source file like ESRI and
    create an actual document.

    In any file format, there will be this problem. There must be a
    temporary object structure while you read in and figure out how big
    the data is. Then, once the data is known, you can read in a single
    contiguous block of memory, fill it and save that out to disk for
    rapid loading later.

    One way to eliminate this problem might be to allocate the header and
    what for us is the segment information for the map with lots of
    extra space at the end. We make the list as big as it needs to be and
    just keep an extra data structure with the points. Then, when done
    growing define a pointer to the data at the end and copy in.

    The uninitialized protected constructor is only for use by children
    who first figure out how big, then call init() to initialize their
    parent. The only alternative would be to call the parent constructor
    passing in the information (type and version is easy, number of bytes
    requires knowing everything before call) This would require returning
    a structure with those 3 items. Should probably just do that.
  */
  struct Info {
    uint64_t bytes;
    Type t;
    uint32_t version;
  };
  BlockLoader(const Info& info);
  uint64_t crc64(uint32_t start, uint32_t stride) const;
  void hashThisDocument() const;
  BlockLoader(const char filename[], uint64_t fileSize, uint64_t memSize = 0);
 public:
  ~BlockLoader() {
    // std::cerr << "destroying: " << mem << std::endl;
    free(mem);
  }
  BlockLoader(const BlockLoader& orig) = delete;
  BlockLoader& operator=(const BlockLoader& orig) = delete;
  BlockLoader(BlockLoader&& orig)
      : mem(orig.mem), size(orig.size), generalHeader(orig.generalHeader) {
    //orig.mem = nullptr;
    free(orig.mem);
  }

  static void* findNextAlign8(char* p){
    return (void*)((uint64_t)(p + 7) / 8 * 8);
  }
  // static BlockLoader readFile(const char filename[], uint64_t memSize);

  // void init(uint64_t* mem, uint64_t size);
  // void init(uint64_t bytes, Type t, uint32_t version);
  //  Fast load a blockfile
  void readBlockFile(const char filename[]);
  uint32_t getHeaderSize() const { return sizeof(GeneralHeader); }
  void* getSpecificHeader() const {
    return (char*)mem + sizeof(GeneralHeader) + generalHeader->header_size;
  }  // TODO: do we need header_size at all? variable sized headers?

  void registerDocument(uint64_t author_id) const;
  // register this document with a server under author's id
  bool authenticateDocument()
      const;  // return true if this document is correctly signed on server

  void writeFile(const char filename[], uint64_t fileSize);
  void writeFile(const char filename[]);

  BlockLoader(uint64_t bytes, Type t, uint16_t version);
};
