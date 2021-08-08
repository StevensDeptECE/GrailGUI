#include <cassert>
#include <iostream>
#include <list>

#include "xp/Stats.hh"

using stats::QuantileAlgorithm;

using namespace std;

inline bool compare_double(double a, double b, double epsilon = 0.01f) {
  return std::abs(a - b) < epsilon;
}

int main() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};

  double mean = stats::mean(data);
  double pstdev = stats::pstdev(data, mean);
  double stdev = stats::stdev(data, mean);
  double pvariance = stats::pvariance(data, mean);
  double variance = stats::variance(data, mean);

  cout << mean << endl
       << pstdev << endl
       << stdev << endl
       << pvariance << endl
       << variance << endl;

  assert(compare_double(mean, 186.731, 0.001));
  assert(compare_double(pstdev, 95.3574, 0.0001));
  assert(compare_double(stdev, 97.2459, 0.0001));
  assert(compare_double(pvariance, 9093.04, 0.01));
  assert(compare_double(variance, 9456.76, 0.01));

  double q1 = stats::quantile(data, .25, QuantileAlgorithm::R5);
  // assert(is_between(q1, 100, 100));

  // assert(is_between(stats::quantile(data, .75, stats::QuantileAlgorithm::R5),
  //                   270, 270));

  // double median =
  //     stats::five_number_summary(data, stats::QuantileAlgorithm::R5).median;
  // assert(is_between(median, 185, 185));

  // assert(is_between(stats::iqr(data, stats::QuantileAlgorithm::R5), 169.99,
  //                   170.1));

  for (auto const& n : stats::multimode(data)) cout << n << endl;
}
