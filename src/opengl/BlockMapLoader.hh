#pragma once
#include <memory>
class BlockLoader {
 public:
  std::unique_ptr<uint64_t> mem;
  struct GeneralHeader {
    uint32_t magic;    // magic number for all block loaders
    uint32_t type;     // type of block loader
    uint32_t version;  // version
    uint32_t unused;
  };
  struct SecurityHeaderV0 {};
#if 0
	struct SecurityHeaderV1 {
		uint8_t hash[32];  // SHA256 hash signed by author
		uint8_t hash2[32]; // SHA256 hash of alternate region in data signed by author
		uint8_t id[32]; // id of author
	};
#endif

  GeneralHeader* generalHeader;
  SecurityHeaderV0* securityHeader;
  enum class Type {
    gismap,
    hashmap
  };
  // get the size of the SecurityHeader for any BlockLoader
  uint32_t getAuthHeaderSize() const { return sizeof(SecurityHeaderV0); }
  uint32_t getHeaderSize() const { return sizeof(GeneralHeader) + sizeof(SecurityHeaderV0); }
  bool authenticate() const;
  GeneralHeader* generalHeader;
  BlockLoader(uint64_t bytes, Type t, uint32_t version) : mem(std::make_unique<uint64_t>(new uint64_t[getHeaderSize() + (bytes + 7) / 8])) {
    generalHeader = (GeneralHeader*)mem;                                    // header is the first chunk of bytes
    generalHeader->magic = ((((('!' << 8) + 'B') << 8) + 'L') << 8) + 'd';  // magic number for all block loaders
    generalHeader->type = uint32_t(t);
    generalHeader->version = version;
    securityHeader = (SecurityHeader*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
  }
};

class BlockMapLoader : public BlockLoader, public ESRIShape {
 private:
  SpecificHeader* specificHeader;
  const float* points;
  static constexpr uint32_t version = 0x00010000;

 public:
  struct SpecificHeader {
    uint32_t numLists;
    uint32_t unused;
  };

  struct Segment {
    uint32_t numPoints;
    uint32_t type : 8;
    uint32_t unused : 24;
    double baseLocX, baseLocY;
  };
  BlockMapLoader(uint32_t numLists, uint32_t numPoints);
  BlastMapLoader(const char filename[]);
	void save(const char filename[]);
	void filterX(double xMin, double xMax);
	void filterY(double yMin, double yMax);
	void filter(double xMin, double xMax, double yMin, double yMax);
};
