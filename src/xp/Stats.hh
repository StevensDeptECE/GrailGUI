#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>

template <typename T>
class Stats1D {
 public:
  struct Summary {
    double min, max, q1, q3, median;
  };

 private:
  constexpr static uint8_t SORTED = 0b00100000;
  constexpr static uint8_t MEAN = 0b00010000;
  constexpr static uint8_t STDDEV = 0b00001000;
  constexpr static uint8_t VARIANCE = 0b00000100;
  constexpr static uint8_t IQR = 0b00000010;
  constexpr static uint8_t FIVENUM = 0b00000001;

  std::vector<T> sorted_data;
  // 0b00111111
  // sorted, mean, stddev, variance, iqr, fivenum
  uint8_t cache_flags;
  std::unique_ptr<double> mean, stddev, variance, iqr;
  std::vector<T> modes;

  std::unique_ptr<struct Summary> fivenum;

 public:
  Stats1D() = delete;

  template <typename FowardIter>
  Stats1D(FowardIter a, const FowardIter b, bool sorted = false)
      : cache_flags(0b00100000) {
    sorted_data = std::vector<T>(a, b);

    if (!sorted) {
      sort(sorted_data.begin(), sorted_data.end());
    }
  }

  template <typename Iterable>
  Stats1D(const Iterable& container, bool sorted = false)
      : Stats1D(std::begin(container), std::end(container), sorted) {}

  Stats1D(const Stats1D<T>& orig) {
    sorted_data = orig.sorted_data;
    cache_flags = 0b00100000;
  }

  Stats1D& operator=(const Stats1D<T>& orig) {
    if (this != &orig) {
      std::cout << "assignment copy" << std::endl;
      sorted_data = orig.sorted_data;
      cache_flags = 0b00100000;
    }

    return *this;
  }

  template <typename FowardIter>
  void updateData(FowardIter a, const FowardIter b, bool sorted = false);

  template <typename Iterable>
  void updateData(const Iterable& container, bool sorted = false);

  double getMean();
  std::vector<T> getModes();
  double getIQR();
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

  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
  }

  cache_flags = 0b00100000;

  modes.clear();
}

template <typename T>
template <typename Iterable>
void Stats1D<T>::updateData(const Iterable& container, bool sorted) {
  updateData(std::begin(container), std::end(container), sorted);
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
  if ((cache_flags & MEAN) == MEAN) return *mean;

  double mean_tmp = 0;

  for (auto const& num : sorted_data) {
    mean_tmp += num;
  }

  mean_tmp /= sorted_data.size();

  mean = std::make_unique<double>(mean_tmp);

  cache_flags |= MEAN;

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
  if (modes.size() != 0) return modes;

  std::unordered_map<T, int> map;

  double biggest_mode = 1;

  for (auto const& num : sorted_data) {
    int tmp = 1;
    auto search = map.find(num);

    if (search != map.end()) {
      tmp = search->second + 1;
    }

    map.insert_or_assign(num, tmp);

    if (tmp > biggest_mode) {
      biggest_mode = tmp;
    }
  }

  for (auto const& [key, value] : map) {
    if (value == biggest_mode) {
      modes.push_back(key);
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
double Stats1D<T>::getIQR() {
  if ((cache_flags & IQR) == IQR) return *iqr.get();

  Stats1D<T>::getSummary();
  struct Summary fn = *fivenum.get();
  iqr = std::make_unique<double>(fn.q3 - fn.q1);
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
  if ((cache_flags & FIVENUM) == FIVENUM) return *fivenum.get();

  struct Summary fn;
  fn.min = *sorted_data.begin();
  fn.max = *(sorted_data.end() - 1);

  fn.q1 = getQuantile(.25);
  fn.median = getQuantile(.50);
  fn.q3 = getQuantile(.75);

  fivenum = std::make_unique<struct Summary>(fn);

  cache_flags |= FIVENUM;

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
  if ((cache_flags & STDDEV) == STDDEV) return *stddev.get();
  stddev = std::make_unique<double>(sqrt(getVariance()));

  cache_flags |= STDDEV;

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
  if ((cache_flags & VARIANCE) == VARIANCE) return *variance.get();

  double mean_tmp = getMean();
  double sum = 0;
  for (auto const& num : sorted_data) {
    sum += pow(num - mean_tmp, 2);
  }
  variance = std::make_unique<double>(sum / (sorted_data.size() - 1));

  cache_flags |= VARIANCE;

  return *variance.get();
}

/**
 * @brief getQuantile - Gets a quantile of the sorted array
 *
 * This implements the R-6 algorithm for finding quantiles. Upon reviewing the
 * relevant paper, the index functions refer to an array with a starting index
 * of 1, but C++ is 0-indexed. As such, the added one that is expected in R-7
 * has been negated. (Hyndman and Fan, 1997).
 *
 * @param percentile The percentile to look for
 * @return double The resultant quantile
 **/
template <typename T>
double Stats1D<T>::getQuantile(double percentile) {
  double h = (sorted_data.size() + 1) * percentile - 1;
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
     << "\nIQR: " << stats.getIQR() << "\nModes: [";

  std::vector<T> modes = stats.getModes();
  for (auto const& i : modes) {
    os << i << ", ";
  }
  os << "]";
  return os;
}
