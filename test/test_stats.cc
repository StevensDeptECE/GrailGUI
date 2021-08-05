#include <cassert>
#include <iostream>
#include <list>

#include "xp/stats.h"

#define is_between(num, lower, upper) ((num >= lower) && (num <= upper))

using namespace std;
int main() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};

  assert(is_between(stats::mean(data), 186.72, 186.74));

  assert(is_between(stats::pstdev(data), 95.35, 95.36));

  assert(is_between(stats::stdev(data), 97.24, 97.25));

  assert(is_between(stats::pvariance(data), 9093.03, 9093.05));

  assert(is_between(stats::variance(data), 9456.75, 9456.77));

  double q1 = stats::quantile(data, .25, stats::QuantileAlgorithm::R5);
  assert(is_between(q1, 100, 100));

  assert(is_between(stats::quantile(data, .75, stats::QuantileAlgorithm::R5),
                    270, 270));

  double median =
      stats::five_number_summary(data, stats::QuantileAlgorithm::R5).median;
  assert(is_between(median, 185, 185));

  // assert(is_between(stats::iqr(), 169.99, 170.1));
}
