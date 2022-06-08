#include "util/FileUtil.hh"
#include "util/Benchmark.hh"


uint32_t slowBytewiseCountWords(const char* dict, uint32_t len) {
  uint32_t count = 0;
  for (int i = 0; i < len; i++)
    if (dict[i] == '\n') // newline = ^J character code =10   cr = \r  ^M  = 13
      count++;
  return count;
}

uint32_t fastWordwiseCountWords(const char* dict, uint32_t len) {
  uint32_t count = 0;
  const uint64_t* p = (const uint64_t*)dict;
  const uint32_t size = len / 8; // rounded down, number of full words
  // assumption: little-endian
  for (int i = 0; i < size; i++) {
    uint64_t current = *p++;
    for (int byte = 8; byte > 0; byte--, current >>= 8) {
      if (current & 0xFF == '\n')
        count++;
    }
  }
  return count;
}

const uint32_t ENDBIT = 0b11111; // 31 = special END value for 5-bit letter encoding
uint32_t fast5bitCountWords(const char* dict, uint32_t len) {
  uint32_t count = 0;
  const uint64_t* p = (const uint64_t*)dict;
  const uint32_t size = len / 8; // rounded down, number of full words
  // assumption: little-endian
  for (int i = 0; i < size; i++) {
    uint64_t current = *p++;
    for (int byte = 8; byte > 0; byte--, current >>= 5) {
      if (current & 0x1F == ENDBIT)
        count++;
    }
  }
  return count;
}
//arithmetic encoding storing each letter as 0..25, END1 = 26, END2 = 27 base 28
const uint32_t END1 = 26; // special end value
uint32_t fastArithmeticEncodedCountWords(const char* dict, uint32_t len) {
  uint32_t count = 0;
  const uint64_t* p = (const uint64_t*)dict;
  const uint32_t size = len / 8; // rounded down, number of full words
  // assumption: little-endian
  for (int i = 0; i < size; i++) {
    uint64_t current = *p++;
    for (int byte = 8; byte > 0; byte--, current /= 28) {
      if (current % 28 == END1)
        count++;
    }
  }
  return count;
}





int main() {
  uint32_t len;
  char* dict;
  FileUtil::readComplete(&dict,  &len, "xp/dict.txt");
  //Assumption: dict is allocated on even 8-byte boundary
  //dictionary contains words separated by \n
  // aa\naal\n...
  using namespace grail::utils;
  uint32_t totalCount = 0;
  CBenchmark<>::benchmark("bytewise loading of dict", 1e2,
        [&]() { totalCount += slowBytewiseCountWords(dict, len); } );
  std::cout << "totalCount:" << totalCount << '\n';
  CBenchmark<>::benchmark("wordwise loading of dict", 1e2,
        [&]() { totalCount += fastWordwiseCountWords(dict, len); } );
  std::cout << "totalCount:" << totalCount << '\n';
  
    
}