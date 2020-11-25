#include "Benchmark.hh"
#include <unistd.h>
#include <sys/times.h>

using namespace std;

static struct tms startCPUTimes;
static time_t startWallTime;
static struct tms endCPUTimes;
static time_t endWallTime;
static clock_t t;

Benchmark::Benchmark() :
  elapsedUserTime(0), elapsedSystemTime(0), elapsedWallTime(0), elapsedClockTicks(0),
  BENCHCLOCKS_PER_SEC(sysconf(_SC_CLK_TCK)) {
}


void Benchmark::start() {
  times(&startCPUTimes);
  time(&startWallTime);

  t = clock();
}

void Benchmark::end() {
  times(&endCPUTimes);
  time(&endWallTime);
  elapsedUserTime += endCPUTimes.tms_utime - startCPUTimes.tms_utime;
  elapsedSystemTime += endCPUTimes.tms_stime - startCPUTimes.tms_stime;
  elapsedWallTime += endWallTime - startWallTime;

  elapsedClockTicks = clock() - t;
}

void Benchmark::benchmark(void (*Func)() ) {
  Benchmark a;
  a.start();
  Func();
  a.end();
  std::cout << a << '\n';
}

void Benchmark::benchmark(void (*Func)(int), int n ) {
  Benchmark a;
  a.start();
  Func(n);
  a.end();
  std::cout << a << '\n';
}

ostream& operator <<(ostream& s, const Benchmark& b) {
  return
    s <<
    "User:        " << b.elapsedUserTime / double(b.BENCHCLOCKS_PER_SEC) << '\n' <<
    "System:      " << b.elapsedSystemTime  / double(b.BENCHCLOCKS_PER_SEC) << '\n' <<
    "Wall:        " << b.elapsedWallTime << '\n' <<
    "Clock Ticks: " << b.elapsedClockTicks << '\n';

}
