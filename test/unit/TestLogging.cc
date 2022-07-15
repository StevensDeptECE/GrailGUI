#include <fstream>

#include "util/Log.hh"

int main() {
  std::ofstream filestr("test.txt");
  Logger fl(&filestr);

  fl.error("error");
  filestr.close();
}