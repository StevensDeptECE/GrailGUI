#include <memory>
#include <vector>

template <typename T>
class Stats1D {
 private:
  T* array;
  uint32_t size;
  bool sorted;
  unique_ptr<T> median, mean, iqr, stddev, variance, q1, q3;
  vector<T> modes;

  struct summary {
    T min, max, q1, q3, median;
  };

  unique_ptr<summary> fivenum;

 public:
  Stats1D(T* array, uint32_t size, sorted = false)
      : array(array), size(size), sorted(sorted){};
  T getMean() const;
  std::vector<T> getModes() const;
  T getIQR() const;
  summary getSummary() const;
  T getStdDev() const;
  T getVariance() const;
};