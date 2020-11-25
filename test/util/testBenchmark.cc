#include "util/Benchmark.hh"
#include "csp/CSPConfig.hh"
#include "csp/CSPRequest.hh"
#include "csp/Socket.hh"
#include "csp/IPV4Socket.hh"
#include "csp/Request.hh"
#include "csp/CSPClient.hh"

int n;
const char* addr;

// function to benchmark length of each process
void testBenchmark(){
	//Benchmark::benchmark(clientWriteForTest);
	//Benchmark::benchmark(clientReadForTest);
	CSPClient client(addr);
	Socket* s = new IPV4Socket("127.0.0.1", "");
	s->attach(new CSPRequest(*s));
	
	for (int i = 0; i < n; i++){
		cout<< "test: "<< i<< "\n";
		s->send();
	}
}

int main(int argc, char *argv[]){
		if (argc < 3) {
			cerr << "Usage: BMARK url numTrials\n";
			return -1;
		}
		addr = argv[1];
		n = argc > 1 ? atoi(argv[2]) : 1000;
		Benchmark bmark;
		bmark.benchmark(testBenchmark);
		return 0;
}
