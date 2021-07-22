#include <cassert>
#include <list>

#include "xp/Stats.hh"

using namespace std;

#define is_between(num, lower, upper) ((num >= lower) && (num <= upper))

void test_contructor() {
  int array[] = {1, 2, 2, 3, 3, 4, 5};

  Stats1D<int> stats_from_array = Stats1D<int>(array);

  int* dynarray = new int[5]{1, 2, 3, 4, 5};

  Stats1D<float> stats_from_dynarray = Stats1D<float>(dynarray, dynarray + 5);

  std::vector<int> vec{1, 2, 3, 4, 5};

  Stats1D<double> stats_from_vec = Stats1D<double>(vec);
  Stats1D<int> stats_from_vec_begin = Stats1D<int>(vec.begin(), vec.end());

  std::list<uint64_t> list{1, 2, 3, 4, 5};

  Stats1D<uint64_t> stats_from_list = Stats1D<uint64_t>(list);
}

void test_values() {
  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89};

  Stats1D<double> stats(data);

  assert(is_between(stats.getMean(), 191.4, 191.5));

  assert(is_between(stats.getStdDev(), 96.16, 96.18));

  assert(is_between(stats.getVariance(), 9250.16, 9250.18));

  assert(is_between(stats.getIQR(), 177.99, 178.01));

  assert(is_between(stats.getQuantile(.25), 106.99, 107.01));

  assert(is_between(stats.getQuantile(.75), 284.99, 285.01));

  assert(is_between(stats.getSummary().median, 190, 190));
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

  // int array[] = {1, 2, 2, 3, 3, 4, 5};
  // Stats1D<int> stats_from_array = Stats1D<int>(array);

  // int* dynarray = new int[5]{1, 2, 3, 4, 5};
  // Stats1D<int> stats_from_dynarray = Stats1D<int>(dynarray, dynarray + 5);

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
