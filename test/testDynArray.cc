#include "fmt/core.h"
#include "opengl/GrailGUI.hh"
#include "util/DynArray.hh"

using namespace std;

template <typename T>
constexpr void printarr(string msg, uint32_t size, DynArray<T*> arr) {
  fmt::print("{}", msg);
  for (int i = 0; i < size; ++i) {
    cout << arr[i] << ",";
  }
  fmt::print("\n");
}

template <typename T>
constexpr void printarr(string msg, uint32_t size, DynArray<T> arr) {
  fmt::print("{}", msg);
  for (int i = 0; i < size; ++i) {
    fmt::print("{},", arr[i]);
  }
  fmt::print("\n");
}

int main() {
  DynArray<uint32_t> arr(10);
  for (int i = 0; i < 10; i++) {
    arr.add(i);
  }

  printarr("new array of uint32_t: \n", 10, arr);

  int removedInt = arr.removeAt(5);
  fmt::print("Removed value {} at index 5\n", removedInt);

  printarr("array of uint32_t after deletion: \n", 9, arr);

  GLWin w(1024, 800, 0xFFFFFFFF, 0x000000FF, "gw");
  DynArray<Tab*> tabs(4);
  for (int i = 0; i < 4; i++) {
    tabs.add(new Tab(&w));
  }

  printarr("new array of tabs: \n", 4, tabs);

  Tab* removedTab = tabs.removeAt(2);
  cout << "Removed value " << removedTab << " at index 2\n";

  printarr("array of tabs after deletion: \n", 3, tabs);

  DynArray<string> strs(4);
  strs.add("a");
  strs.add("B");
  strs.add("c");
  printarr("new array of strings: \n", 3, strs);
  string removedString = strs.removeAt(1);
  fmt::print("Removed value {} at index 1\n", removedString);
  printarr("array of tabs after deletion: \n", 2, strs);
}