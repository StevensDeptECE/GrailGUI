#include "visualcs/CSRGraph.hh"

int main() {
  CSRGraph<uint64_t, uint64_t, float> cg("res/email-Eu-core.txt_beg_pos.bin",
                                         "res/email-Eu-core.txt_csr.bin",
                                         nullptr);
  return 0;
}
