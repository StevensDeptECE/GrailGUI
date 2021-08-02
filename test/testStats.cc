#include <cassert>
#include <list>

#include "xp/Stats.hh"

using namespace std;
using namespace stats;

#define is_between(num, lower, upper) ((num >= lower) && (num <= upper))

void test_contructor() {
  int array[] = {1, 2, 2, 3, 3, 4, 5};

  Stats1D<int> stats_from_array(array);

  int* dynarray = new int[5]{1, 2, 3, 4, 5};

  Stats1D<float> stats_from_dynarray(dynarray, dynarray + 5);

  std::vector<int> vec{1, 2, 3, 4, 5};

  Stats1D<double> stats_from_vec(vec);
  Stats1D<int> stats_from_vec_begin(vec.begin(), vec.end());

  std::list<uint64_t> list{1, 2, 3, 4, 5};

  Stats1D<uint64_t> stats_from_list(list);
}

void test_values() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};

  Stats1D<double> stats(data);
  stats.setQuantileAlgorithm(stats::QuantileAlgorithm::R5);

  assert(is_between(stats.getMean(), 186.72, 186.74));

  assert(is_between(stats.getPopulationStdDev(), 95.35, 95.36));

  assert(is_between(stats.getSampleStdDev(), 97.24, 97.25));

  assert(is_between(stats.getPopulationVariance(), 9093.03, 9093.05));

  assert(is_between(stats.getSampleVariance(), 9456.75, 9456.77));

  assert(is_between(stats.getQuantile(.25, stats::QuantileAlgorithm::R5), 100,
                    100));

  assert(is_between(stats.getQuantile(.75, stats::QuantileAlgorithm::R5), 270,
                    270));

  assert(is_between(stats.getSummary().median, 185, 185));

  assert(is_between(stats.getIQR(), 169.99, 170.1));

  /*
  string quant_algs[9] = {"R-1", "R-2", "R-3", "R-4", "R-5",
                          "R-6", "R-7", "R-8", "R-9"};
  for (const auto& elem : quant_algs) {
    stats_from_vec_double.setQuantileAlgorithm(elem);
    cout << "\nQuantile alg " << elem << endl << stats_from_vec_double << endl;
  }*/
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

  // for (auto& summary : v) {
  //   cout << summary << endl;
  // }
}

void test_copy_assignment() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300,
                         250, 190, 170, 100, 50,  20,  150};
  vector<double> other = {200, 330, 200, 270, 180, 300, 49, 247, 325, 114, 89};

  Stats1D<double> a(data);
  std::cout << a << std::endl;

  Stats1D<double> b(other);
  a = b;
  std::cout << a << std::endl;
}

int main() {
  test_contructor();
  test_values();
  test_container_of_stats();
  test_copy_assignment();

  int array[] = {1, 2, 3, 4, 5};
  Stats1D<int> stats_from_array = Stats1D<int>(array);

  int* dynarray = new int[5]{1, 2, 3, 4, 5};
  Stats1D<int64_t> stats_from_dynarray =
      Stats1D<int64_t>(dynarray, dynarray + 5);

  // cout << pearson_correlation(stats_from_array, stats_from_dynarray) << endl;

  // std::vector<int> vec{1, 2, 3, 4, 5};
  // Stats1D<int> stats_from_vec = Stats1D<int>(vec);
  // Stats1D<int> stats_from_vec_begin = Stats1D<int>(vec.begin(), vec.end());

  // std::list<int> list{1, 2, 3, 4, 5};
  // Stats1D<int> stats_from_list = Stats1D<int>(list);

  // cout << stats_from_array << "\n\n"
  //      << stats_from_dynarray << "\n\n"
  //      << stats_from_vec << "\n\n"
  //      << stats_from_vec_begin << "\n\n"
  //      << stats_from_list;
}
