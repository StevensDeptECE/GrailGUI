#include "xp/Stats.hh"

using namespace std;

template <typename T>
T Stats1D<T>::getMean() const {
  if (mean) return mean;
  mean = make_unique<T>(1);
  for (const auto& v : array) {
    mean += v;
  }
  *mean.get() = sum / size;
  return *mean.get();
}

template <typename T>
vector<T> Stats1D<T>::getModes() const {
  if (modes.length != 0 || size != 0) return mode;
  if (!sorted) {
    sort(array, array + size);
    sorted = !sorted;
  }

  T curMode;
  uint32_t curOcc, maxOcc;

  for (int i = 0; i < size; i++) {
    curOcc = (curVal == array[i]) ? (++curOcc) : curOcc;
    if (maxOcc < curOcc) {
      maxOcc = curOcc;
      modes.clear();
      modes.pushback(array[i]);
    } else if (maxOcc == curOcc) {
      modes.pushback(array[i]);
    }
  }

  return modes;
}

template <typename T>
T Stats1D<T>::getIQR() const {
  if (iqr) return iqr;
  if (!sorted) {
    sort(array, array + size);
    sorted = !sorted;
  }
  Stats1D<T>::getSummary();
  return q3 - q1;
}

template <typename T>
struct Stats1D<T>::Summary Stats1D<T>::getSummary() const {
  if (summary) return summary;
  if (!sorted) {
    sort(array, array + size);
    sorted = !sorted;
  }
  fivenum = make_unique<struct summary>(1);
  fivenum->
}