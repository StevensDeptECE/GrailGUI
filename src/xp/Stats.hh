#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>

template <typename T>
class Stats1D {
 public:
  struct Summary {
    double min, max, q1, q3, median;
  };

 private:
  std::vector<T> sorted_data;
  uint32_t size;
  bool sorted;
  std::unique_ptr<double> mean, stddev, variance;
  std::unique_ptr<T> iqr;
  std::vector<T> modes;

  std::unique_ptr<struct Summary> fivenum;
  uint32_t getMedian(uint32_t left, uint32_t right);

 public:
  Stats1D() = delete;

  template <typename FowardIter>
  Stats1D(FowardIter a, const FowardIter b, bool sorted = false) {
    sorted_data = std::vector<T>(a, b);
    size = sorted_data.size();

    if (!sorted) {
      sort(sorted_data.begin(), sorted_data.end());
      sorted = true;
    }
  }

  template <typename Iterable>
  Stats1D(const Iterable& container, bool sorted = false)
      : Stats1D(std::begin(container), std::end(container), sorted) {}

  template <typename FowardIter>
  void updateData(FowardIter a, const FowardIter b, bool sorted);

  // TODO: second updateData method

  double getMean();
  std::vector<T> getModes();
  T getIQR();
  struct Summary getSummary();
  double getStdDev();
  double getVariance();
  double getQuantile(double percentile);
  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, Stats1D<U>& stats);
};

template <typename T>
template <typename FowardIter>
void Stats1D<T>::updateData(FowardIter a, const FowardIter b, bool sorted) {
  sorted_data = std::vector<T>(a, b);
  size = sorted_data.size();
  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
    sorted = true;
  }
}

/**
 * @brief Calculates mean of a numeric type
 *
 * For a dataset without outliers or skew, the mean will represent the
 * center of a dataset. Together with the standard deviation, it is useful
 * for catching extreme values and describing the distribution of the
 * data.
 *
 * @tparam T Any numeric type
 * @return double The mean of a dataset
 */
template <typename T>
double Stats1D<T>::getMean() {
  if (mean) return *mean;
  double mean_tmp = 0;

  for (auto num : sorted_data) {
    mean_tmp += num;
  }

  mean_tmp /= size;

  mean = std::make_unique<double>(mean_tmp);
  return *mean;
}

/**
 * @brief Finds the modes of a numeric type
 *
 * The mode of a dataset is value that most frequently appears.
 *
 * This returns a vector in order to allow the possibility of multiple modes,
 * rather than just returning one of many modes.
 *
 * @tparam T Any numeric type
 * @return vector<T> A vector of modes of a dataset
 */
template <typename T>
std::vector<T> Stats1D<T>::getModes() {
  if (modes.size() != 0 || size == 0) return modes;
  // TODO: shouldn't need to check this here
  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
    sorted = !sorted;
  }

  T curMode;
  uint32_t curOcc, maxOcc;

  // TODO: do with range based loop
  for (int i = 0; i < size; i++) {
    curOcc = (curMode == sorted_data[i]) ? (++curOcc) : curOcc;
    if (maxOcc < curOcc && curOcc != 1) {
      maxOcc = curOcc;
      curMode = sorted_data[i];
      modes.clear();
      modes.push_back(sorted_data[i]);
    } else if (maxOcc == curOcc) {
      modes.push_back(sorted_data[i]);
    }
  }

  return modes;
}

/**
 * @brief Calculates the IQR of a numeric type
 *
 * The interquartile range is the 50% range between the first and third
 * quartile of a dataset. Together with the median of the dataset, it presents
 * an alternative to the mean and standard deviation for finding outliers.
 *
 * @tparam T Any numeric type
 * @return T The IQR of a dataset
 */
template <typename T>
T Stats1D<T>::getIQR() {
  if (iqr) return *iqr.get();
  // TODO: shouldn't need to do this
  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
    sorted = !sorted;
  }
  Stats1D<T>::getSummary();
  struct Summary fn = *fivenum.get();
  iqr = std::make_unique<T>(fn.q3 - fn.q1);
  return *iqr.get();
}

/**
 * @brief Returns a struct containing the five number summary of a numeric
 * type
 *
 * The five number summary contains the minimum, maximum, median, first
 * quartile, and third quartile. These values are useful to describe the
 * distribution of the dataset and find outliers.
 *
 * @tparam T Any generic type
 * @return struct Stats1D<T>::Summary A struct of the five number summary
 */
template <typename T>
struct Stats1D<T>::Summary Stats1D<T>::getSummary() {
  if (fivenum) return *fivenum.get();
  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
    sorted = !sorted;
  }
  struct Summary fn;
  fn.min = *sorted_data.begin();
  fn.max = *(sorted_data.end() - 1);

  fn.q1 = getQuantile(.25);
  fn.median = getQuantile(.50);
  fn.q3 = getQuantile(.75);

  fivenum = std::make_unique<struct Summary>(fn);
  return *fivenum.get();
}

/**
 * @brief Calculates the standard deviation of a numeric type
 *
 * The standard deviation of a dataset describes the spread of a data. A
 * higher standard deviation indicates that the data is spread further from
 * the mean of the dataset.
 *
 * @tparam T Any numeric type
 * @return double The standard deviation of a dataset
 */
template <typename T>
double Stats1D<T>::getStdDev() {
  if (stddev) return *stddev.get();
  stddev = std::make_unique<double>(sqrt(getVariance()));
  return *stddev.get();
}

/**
 * @brief Calculates the variance of a numeric type
 *
 * The variance of a dataset is the square of standard deviation and is
 * another descriptor of the spread of a dataset. Among its many uses are
 * sampling, inference, hypothesis testing, and goodness of fit.
 *
 * @tparam T Any numeric type
 * @return double The variance of a dataset
 */
template <typename T>
double Stats1D<T>::getVariance() {
  if (variance) return *variance.get();
  double mean_tmp = getMean();
  double sum = 0;
  for (auto num : sorted_data) {
    sum += pow(num - mean_tmp, 2);
  }
  variance = std::make_unique<double>(sum / (size - 1));
  return *variance.get();
}

/**
 * @brief getQuantile - Gets a quantile of the sorted array
 *
 * This looks like it implements the R-6 algorithm for finding quantiles, but
 *it is actually R-7. Upon reviewing the relevant paper, the index functions
 *refer to an array with a starting index of 1, but C++ is 0-indexed. As such,
 *the added one that is expected in R-7 has been negated. (Hyndman and Fan,
 *1997).
 *
 * @param percentile The percentile to look for
 * @return double The resultant quantile
 **/
template <typename T>
double Stats1D<T>::getQuantile(double percentile) {
  double h = (sorted_data.size() - 1) * percentile;
  return sorted_data[floor(h)] +
         (h - floor(h)) * (sorted_data[ceil(h)] - sorted_data[floor(h)]);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, Stats1D<T>& stats) {
  struct Stats1D<T>::Summary fivenum = stats.getSummary();
  os << "# Points: " << stats.sorted_data.size()
     << "\nMean: " << stats.getMean() << "\nStdDev: " << stats.getStdDev()
     << "\nVariance: " << stats.getVariance()
     << "\nFive Number Summary:\n\tMinimum: " << fivenum.min
     << "\n\tFirst Quartile: " << fivenum.q1 << "\n\tMedian: " << fivenum.median
     << "\n\tThird Quartile: " << fivenum.q3 << "\n\tMaximum: " << fivenum.max
     << "\nIQR:" << stats.getIQR() << "\nModes: [";

  std::vector<T> modes = stats.getModes();
  for (const auto& i : modes) {
    os << i << ", ";
  }
  os << "]";
  return os;
}
