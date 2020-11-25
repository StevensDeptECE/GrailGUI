
#include <iostream>
#include "util/Config.hh"

class TestConfig : public Config {
public:
	TestConfig() {
		//TODO: support or?  "x|y|z", Config::DOUBLE
		mandatory(6, 
		 	 "x", Config::DOUBLE, 
		 	 "y", Config::DOUBLE, 
		 	 "z", Config::DOUBLE, 
		 	 "port", Config::UINT16, 
		 	 "base", Config::STRING, 
		 	 "foo", Config::STRING);

		optional(2,
						 "foo", Config::STRING,
						 "snarf", Config::UINT64);

}
};

TestConfig test;
void test1(){
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
	} catch(BadType& b) {
		cout << b << '\n';
	}
}
		
