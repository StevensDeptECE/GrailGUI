#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "util/Ex.hh"
#include "xp/Quantile.hh"

struct Summary {
  double min, max, q1, q3, median;
};

static std::string defaultQuantile = "R7";

template <typename T>
class Stats1D {
 private:
  std::vector<T> sorted_data;
  uint32_t size;
  // TODO: this could be done with one single number and bit operations
  bool sorted, mean_calculated, stddev_calculated, variance_calculated,
      iqr_calculated, fivenum_calculated;
  std::unique_ptr<double> mean, stddev, variance, iqr;
  std::vector<T> modes;

  std::unique_ptr<struct Summary> fivenum;
  std::string quantile = defaultQuantile;
  std::unordered_map<std::string, quantile_func<T>> quantile_algorithms;

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

    mean_calculated = false;
    stddev_calculated = false;
    variance_calculated = false;
    iqr_calculated = false;
    fivenum_calculated = false;
    init_quantile_algs(&quantile_algorithms);
  }

  template <typename Iterable>
  Stats1D(const Iterable& container, bool sorted = false)
      : Stats1D(std::begin(container), std::end(container), sorted) {}

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
  double getQuantile(double percentile, std::string quantile_algorithm = "");
  void setQuantileAlgorithm(std::string alg);

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
  mean_calculated = false;
  stddev_calculated = false;
  variance_calculated = false;
  iqr_calculated = false;
  fivenum_calculated = false;
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
  if (mean_calculated) return *mean;

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

  std::unordered_map<T, int> map;

  double biggest_mode = 1;

  for (auto& num : sorted_data) {
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

  for (auto& [key, value] : map) {
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
  if (iqr_calculated) return *iqr.get();

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
struct Summary Stats1D<T>::getSummary() {
  if (fivenum_calculated) return *fivenum.get();

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
  if (stddev_calculated) return *stddev.get();
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
  if (variance_calculated) return *variance.get();
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
 * it is actually R-7. Upon reviewing the relevant paper, the index functions
 * refer to an array with a starting index of 1, but C++ is 0-indexed. As such,
 * the added one that is expected in R-7 has been negated. (Hyndman and Fan,
 * 1997).
 *
 * @param percentile The percentile to look for
 * @return double The resultant quantile
 **/
template <typename T>
double Stats1D<T>::getQuantile(double percentile,
                               std::string quantile_algorithm) {
  std::string alg_str = "";
  if (quantile_algorithm != "") {
    alg_str = quantile_algorithm;
  } else {
    alg_str = quantile;
  }

  auto alg = quantile_algorithms.find(alg_str);
  if (alg != quantile_algorithms.end()) {
    return (alg->second)(sorted_data, percentile);
  } else {
    throw std::runtime_error("Quantile algorithm " + quantile +
                             " not found. Please run init_quantile_algs or "
                             "pass in a valid quantile algorithm.");
  }
}

template <typename T>
void Stats1D<T>::setQuantileAlgorithm(std::string alg) {
  if (alg.length() != 2) throw std::runtime_error("incorrect length");
  if ((alg[0] != 'R' && alg[0] != 'r') || !isdigit(alg[1]) || alg[1] == '0')
    throw Ex1(Errcode::BAD_ARGUMENT);
  quantile = std::string("R") + alg[1];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, Stats1D<T>& stats) {
  struct Summary fivenum = stats.getSummary();
  os << "# Points: " << stats.sorted_data.size()
     << "\nMean: " << stats.getMean() << "\nStdDev: " << stats.getStdDev()
     << "\nVariance: " << stats.getVariance()
     << "\nFive Number Summary:\n\tMinimum: " << fivenum.min
     << "\n\tFirst Quartile: " << fivenum.q1 << "\n\tMedian: " << fivenum.median
     << "\n\tThird Quartile: " << fivenum.q3 << "\n\tMaximum: " << fivenum.max
     << "\nIQR: " << stats.getIQR() << "\nModes: [";

  std::vector<T> modes = stats.getModes();
  for (const auto& i : modes) {
    os << i << ", ";
  }
  os << "]";
  return os;
}
