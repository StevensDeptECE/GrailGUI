#include <list>

#include "xp/Stats.hh"

using namespace std;

int main() {
  int array[5] = {1, 2, 3, 4, 5};
  Stats1D<int> stats_from_array = Stats1D<int>(array);
  std::cout << std::endl;

  int* dynarray = new int[5]{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_dynarray = Stats1D<int>(dynarray, dynarray + 5);
  std::cout << std::endl;

  std::vector<int> vec{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_vec = Stats1D<int>(vec);
  std::cout << std::endl;
  Stats1D<int> stats_from_vec_begin = Stats1D<int>(vec.begin(), vec.end());

  std::cout << std::endl;

  std::list<int> list{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_list = Stats1D<int>(list);

  cout << stats_from_array << "\n\n"
       << stats_from_dynarray << "\n\n"
       << stats_from_vec << "\n\n"
       << stats_from_vec_begin << "\n\n"
       << stats_from_list;
}
