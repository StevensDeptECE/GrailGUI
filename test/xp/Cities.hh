#include "util/BlockMap.hh"
#include "util/BLHashMap.hh"
class Cities : public BLHashMap {
private:
  RegionContainer* regionContainers;
public:
  Cities();
};