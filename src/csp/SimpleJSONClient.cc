#include "csp/csp.hh"
// our application errors must be defined before Ex.hh
#include "csp/IPV4Socket.hh"
#include <chrono>

#include <cstdlib>
#include <vector>
using namespace std;

Log srvlog; // log all important events for security and debugging


struct Point {
	double x,y,z;
};
int main(int argc, char* argv[]) {
	try {
		IPV4Socket s("127.0.0.1", 8000);

		auto start = chrono::high_resolution_clock::now();
		s.send(0);
		Buffer& in = s.getIn();
		vector<Point> points;
		uint32_t n = in.parseU32();
		points.reserve(n);
		for (int i = 0; i < n; i++) {
			double x = in.parseF64();
			double y = in.parseF64();
			double z = in.parseF64();
			points.push_back(Point(x,y,z));
		}			
		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
		cout << "\nTotal time in microseconds: " << duration.count();
	} catch (const Ex& e) {
		cerr << e << '\n';
	}
	return 0;
}
