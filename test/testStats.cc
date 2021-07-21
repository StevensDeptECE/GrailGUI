#include <cassert>
#include <list>

#include "xp/Stats.hh"

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))

int main() {
  int array[] = {1, 2, 2, 3, 3, 4, 5};
  Stats1D<int> stats_from_array = Stats1D<int>(array);

  int* dynarray = new int[5]{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_dynarray = Stats1D<int>(dynarray, dynarray + 5);

  std::vector<int> vec{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_vec = Stats1D<int>(vec);
  Stats1D<int> stats_from_vec_begin = Stats1D<int>(vec.begin(), vec.end());

  std::list<int> list{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_list = Stats1D<int>(list);

  cout << stats_from_array << "\n\n"
       << stats_from_dynarray << "\n\n"
       << stats_from_vec << "\n\n"
       << stats_from_vec_begin << "\n\n"
       << stats_from_list;
}
