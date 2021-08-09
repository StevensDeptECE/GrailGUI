#include <time.h>

#include <cassert>
#include <list>

#include "xp/Stats.hh"

using namespace std;
using namespace stats;

#define is_between(num, lower, upper) ((num >= lower) && (num <= upper))

void test_contructor() {
  int array[]{1, 2, 2, 3, 3, 4, 5};

  Stats1D<int> stats_from_array(array);

  int* dynarray = new int[5]{1, 2, 3, 4, 5};

  Stats1D<float> stats_from_dynarray(dynarray, dynarray + 5);

  std::vector<int> vec{1, 2, 3, 4, 5};

  Stats1D<double> stats_from_vec(vec);
  Stats1D<int> stats_from_vec_begin(vec.begin(), vec.end());

  std::list<uint64_t> list{1, 2, 3, 4, 5};

  Stats1D<uint64_t> stats_from_list(list);

  Stats1D<int> stats_from_inline{0, 1, 2, 3, 4, 5};
}

void test_values() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};

  Stats1D<double> stats(data);
  stats.set_quantile_alg(stats::QuantileAlgorithm::R5);

  assert(is_between(stats.mean(), 186.72, 186.74));

  assert(is_between(stats.pstdev(), 95.35, 95.36));

  assert(is_between(stats.stdev(), 97.24, 97.25));

  assert(is_between(stats.pvariance(), 9093.03, 9093.05));

  assert(is_between(stats.variance(), 9456.75, 9456.77));

  assert(
      is_between(stats.quantile(.25, stats::QuantileAlgorithm::R5), 100, 100));

  assert(
      is_between(stats.quantile(.75, stats::QuantileAlgorithm::R5), 270, 270));

  assert(is_between(stats.five_number_summary().median, 185, 185));

  assert(is_between(stats.iqr(), 169.99, 170.1));
}

// We've had undefined behavior in the past from this
void test_container_of_stats() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89};

  vector<Stats1D<double>> v = vector<Stats1D<double>>();

  for (int i = 0; i < data.size(); i += 5) {
    auto first = data.begin() + i;
    auto last = data.begin() + i + 5;
    vector<double> currentData(first, last);
    Stats1D<double> summary(currentData);
    v.push_back(summary);
  }

  for (auto& summary : v) {
    cout << summary.five_number_summary().max << endl;
  }
}

void test_copy_assignment() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300,
                         250, 190, 170, 100, 50,  20,  150};
  vector<double> other = {200, 330, 200, 270, 180, 300, 49, 247, 325, 114, 89};

  Stats1D<double> a(data);
  cout << a << endl;

  Stats1D<double> b(other);
  a = b;
  cout << a << endl;
}

void test_update_data() {
  Stats1D<int> v{150, 350, 222, 100, 300, 130, 300,
                 250, 190, 170, 100, 50,  20,  150};
  vector<int> new_data{200, 330, 200, 270, 180, 300, 49, 247, 325, 114, 89};

  double old_variance = v.variance();

  v.update_data(new_data);

  double new_variance = v.variance();

  assert(old_variance != new_variance);

  v.update_data(
      {150, 350, 222, 100, 300, 130, 300, 250, 190, 170, 100, 50, 20, 150});

  double newer_new_variance = v.variance();

  assert(old_variance == newer_new_variance);
}

int main() {
  test_contructor();
  test_values();
  test_container_of_stats();
  test_copy_assignment();

  // constexpr uint64_t n = 100000000;
  // double* x = new double[n];
  // for (uint64_t i = 0; i < n; i++) x[i] = i;
  // Stats1D<double> stat(x, x + n);
  // clock_t t0 = clock();
  // double sum = stat.variance();
  // clock_t t1 = clock();
  // cout << "Elapsed: " << (t1 - t0) << '\n';
  // cout << "sum=" << sum << '\n';
  // delete[] x;
}
