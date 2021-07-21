#include <lzma.h>

#include <util/Ex.hh>

// TODO: Add error handling for lzma
void lzma_err(lzma_ret return_val) {
  if (return_val != LZMA_OK && return_val != LZMA_STREAM_END) {
    std::cerr << return_val << '\n';
    throw "lzma error";
  }
}
