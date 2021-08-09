#include <cassert>
#include <iostream>
#include <list>

#include "xp/Stats.hh"

using namespace stats::aliases;

using namespace std;

inline bool compare_double(double a, double b, double epsilon = 0.01f) {
  return std::abs(a - b) < epsilon;
}

int main() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};

  double mean = stats::mean(data);
  double median = stats::median(data).value();
  double pstdev = stats::pstdev(data, mean);
  double stdev = stats::stdev(data, mean);
  double pvariance = stats::pvariance(data, mean);
  double variance = stats::variance(data, mean);

  cout << mean << endl
       << median << endl
       << pstdev << endl
       << stdev << endl
       << pvariance << endl
       << variance << endl
       << endl;

  assert(compare_double(mean, 186.731, 0.001));
  assert(compare_double(median, 185, 0.01));
  assert(compare_double(pstdev, 95.3574, 0.0001));
  assert(compare_double(stdev, 97.2459, 0.0001));
  assert(compare_double(pvariance, 9093.04, 0.01));
  assert(compare_double(variance, 9456.76, 0.01));

  double q1 = stats::quantile(data, .25, QuantileAlgorithm::R5);
  double q3 = stats::quantile(data, .75, QuantileAlgorithm::R5);

  cout << q1 << endl << q3 << endl << endl;

  assert(compare_double(q1, 100));
  assert(compare_double(q3, 270));

  cout << stats::mode(data) << endl;
  for (auto const& n : stats::multimode(data)) cout << n << endl;

  std::vector<double> empty;
  try {
    double mn = stats::mean(empty);
  } catch (const stats_error& e) {
    cout << e.what() << endl;
  }
}
