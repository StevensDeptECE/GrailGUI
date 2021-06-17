#include <memory>
#include <vector>

template <typename T>
class Stats1D {
 private:
  T* array;
  std::vector<T> sorted_array;
  uint32_t size;
  bool sorted;
  std::unique_ptr<double> mean, stddev, variance;
  std::unique_ptr<T> iqr;
  std::vector<T> modes;

  struct Summary {
    T min, max, q1, q3, median;
  };

  std::unique_ptr<struct summary> fivenum;
  uint32_t getMedian(uint32_t left, uint32_t right);

 public:
  Stats1D() = delete;
  Stats1D(T* newArray, uint32_t newSize, bool sorted = false);
  void updateArray(T* array, uint32_t size, bool sorted = false);
  double getMean() const;
  std::vector<T> getModes() const;
  T getIQR() const;
  struct Summary getSummary() const;
  double getStdDev() const;
  double getVariance() const;
};