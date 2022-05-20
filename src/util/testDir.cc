#include "FileUtil.hh"
using namespace std;

class A {
 public:
  void doit(const char filename[]) { cout << filename << '\n'; }
};

int main() {
  regex r("\\.otf|\\.ttf", std::regex_constants::icase);
  A a1;
  FileUtil::findAll(&r, "../../conf/fonts/", &a1, &A::doit);
}
