#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <typename iterable_type>
class BabyStats {
 private:
  std::vector<iterable_type> internal_vector;
  bool sorted;

 public:
  BabyStats() = delete;

  template <class FWIt>
  BabyStats(FWIt a, const FWIt b) {
    // while (a != b) {
    //   std::cout << "Value: " << *a << '\n';
    //   ++a;
    // }

    internal_vector = vector<iterable_type>(a, b);
    std::cout << internal_vector[0] << std::endl;
  }

  template <class T>
  BabyStats(const T& container) {
    using std::begin;
    using std::end;
    BabyStats(begin(container), end(container));
  }
};

int main() {
  int array[5] = {1, 2, 3, 4, 5};
  BabyStats<int> stats_from_array = BabyStats<int>(array);
  std::cout << std::endl;

  int* dynarray = new int[5]{1, 2, 3, 4, 5};
  BabyStats<int> stats_from_dynarray = BabyStats<int>(dynarray, dynarray + 5);
  std::cout << std::endl;

  std::vector<int> vec{1, 2, 3, 4, 5};
  BabyStats<int> stats_from_vec = BabyStats<int>(vec);
  std::cout << std::endl;
  BabyStats<int> stats_from_vec_begin = BabyStats<int>(vec.begin(), vec.end());

  std::cout << std::endl;

  std::list<int> list{1, 2, 3, 4, 5};
  BabyStats<int> stats_from_list = BabyStats<int>(list);
}
