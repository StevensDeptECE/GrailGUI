#include <cassert>
#include <list>

#include "xp/Stats.hh"

using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))

int main() {
  defaultQuantile = "R6";
  int array[] = {1, 2, 2, 3, 3, 4, 5};
  Stats1D<int> stats_from_array = Stats1D<int>(array);

  int* dynarray = new int[5]{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_dynarray = Stats1D<int>(dynarray, dynarray + 5);

  std::vector<int> vec{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_vec = Stats1D<int>(vec);
  Stats1D<int> stats_from_vec_begin = Stats1D<int>(vec.begin(), vec.end());

  std::list<int> list{1, 2, 3, 4, 5};
  Stats1D<int> stats_from_list = Stats1D<int>(list);

  vector<double> data = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};
  vector<float> data2 = {150, 350, 222, 100, 300, 130, 300, 250, 190,
                         170, 100, 50,  20,  150, 200, 330, 200, 270,
                         180, 300, 49,  247, 325, 114, 89,  69};
  Stats1D<double> stats_from_vec_double(data);
  Stats1D<float> stats_from_vec_float(data2);
  stats_from_vec_double.getQuantile(.10);
  cout << stats_from_array << "\n\n"
       << stats_from_dynarray << "\n\n"
       << stats_from_vec << "\n\n"
       << stats_from_vec_begin << "\n\n"
       << stats_from_list << "\n\n"
       << stats_from_vec_double << "\n\n"
       << stats_from_vec_float << "\n\n";

  string quant_algs[9] = {"R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9"};
  for (const auto& elem : quant_algs) {
    stats_from_vec_double.setQuantileAlgorithm(elem);
    cout << "\nQuantile alg " << elem << endl << stats_from_vec_double << endl;
  }
}
