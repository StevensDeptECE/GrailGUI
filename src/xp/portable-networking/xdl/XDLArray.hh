#include "util/DynArray.hh"
#include "xdl/std.hh"

class XDLArray : public DynArray<uint8_t> {
 private:
  /* data */
 public:
  XDLArray(uint32_t size = 32) : DynArray(size) {}
  ~XDLArray();
  void addString(const std::string& str);
  void addStruct(const char name[], uint8_t num_elements);
  void addBuiltinMember(DataType t, const char str[]);
  void addMember(DataType t, const char str[]);
};
