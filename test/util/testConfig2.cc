
#include <iostream>

#include "util/Config.hh"

class TestConfig : public Config {
 public:
  TestConfig() {
    // TODO: support or?  "x|y|z", Config::DOUBLE
    mandatory(6, "x", Config::Type::F64, "y", Config::Type::F64, "z",
              Config::Type::F64, "port", Config::Type::U16, "base",
              Config::Type::STRING, "foo", Config::Type::STRING);

    optional(2, "foo", Config::Type::STRING, "snarf", Config::Type::U64);
  }
};

TestConfig test;
void test1() {
  test.load("data/test.conf");
  uint16_t port = test.getUInt16("port");
  string foo = test.getString("foo");
  double x = test.getDouble("x");
  double y = test.getDouble("y");
  double z = test.getDouble("z");

  cout << port << '\n' << foo << '\n' << x << '\n' << y << '\n' << z << '\n';
  x += 9.5;
  //	test.set("x", x);
  test.save("data/testcopy.conf");
}

int main() {
  try {
    test1();
  } catch (BadType& b) {
    cout << b << '\n';
  }
}
