#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace stats {

struct Summary {
  double min, max, q1, q3, median;
};
enum class QuantileAlgorithm { R1 = 0, R2, R3, R4, R5, R6, R7, R8, R9 };

template <typename T>
class Stats1D {
 public:
  /**
   * @brief Construct a new Stats1D object
   *
   * @tparam FowardIter
   * @param a
   * @param b
   * @param sorted
   */
  template <typename FowardIter>
  Stats1D(FowardIter a, const FowardIter b, bool sorted = false);

  /**
   * @brief Construct a new Stats1D object
   *
   * @tparam Iterable
   * @param container
   * @param sorted
   */
  template <typename Iterable>
  Stats1D(const Iterable& container, bool sorted = false);

  /**
   * @brief Update the internal data of the current object
   *
   * @details Replaces the internal original_data and sorted_data with the
   * values supplied by the iterators. This will also reset the cache_flags,
   * so various values will need to be recalculated when functions are
   * called again.
   *
   * @tparam FowardIter
   * @param a
   * @param b
   * @param sorted
   */
  template <typename FowardIter>
  void update_data(FowardIter a, const FowardIter b, bool sorted = false);

  /**
   * @brief Update the internal data of the current object
   *
   * @details Replaces the internal original_data and sorted_data with the
   * values supplied by the parameter. This will also reset the cache_flags, so
   * various values will need to be recalculated when functions are called
   * again.
   *
   *
   * @tparam Iterable
   * @param container
   * @param sorted
   */
  template <typename Iterable>
  void update_data(const Iterable& container, bool sorted = false);

  /**
   * @brief Returns the length of the internally stored data
   *
   * @return int
   */
  int size() { return original_data.size(); }

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
  double mean();

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
  std::vector<T> multimode();

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
  double iqr();

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
  Summary five_number_summary();

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
  double stdev();

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
  double pstdev();

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
  double variance();

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
  double pvariance();

  /**
   * @brief getQuantile - Gets a quantile of the sorted array
   *
   * This uses Quantile.hh to calculate quantiles based on the desired quantile
   * algorithm. By default, R-7 is used, but this can be changed for the object
   *by either setting the defaultQuantile string before creating a Stats1D
   *object or by calling Stats1D<T>::setQuantileAlgorithm.
   *
   * @param percentile The percentile to look for
   * @param quantile_algorithm An optional string in the form of "R-[0-9]"
   * @return double The resultant quantile
   **/
  double quantile(double percentile, QuantileAlgorithm alg);

  /**
   * @brief setQuantileAlgorithm - Sets the quantile algorithm used locally
   *
   * This allows the user to set the quantile algorithm manually, and on a
   * per-object basis in order to ensure that they are getting the results they
   * expect each time they get a quantile or generate a five number summary.
   *
   * An example use-case for this involves a need for getting quantiles that
   *match the data, in order to use the result to query a database or a hashmap.
   *In this instance, R-1 thorough R-3 would be ideal as they do no
   *interpolation between array elements. Alternatively, if linear interpolation
   *is okay and the data is approximately normal, then R-9 is approximately
   *unbiased for the expected order statistic. Hyndman and Fan have recommended
   *R-8 as the algorithm of choice for finding quantiles, but due to the more
   *frequent use of R-6 and R-7, as well as the lack of division in the
   *calculation, we have chosen to use R-7.
   *
   * @param alg The algorithm of choice, as a string in the form of "R-[0-9]"
   **/
  void set_quantile_alg(QuantileAlgorithm alg);

  T operator[](int i) const { return original_data[i]; }

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, Stats1D<U>& stats);

 private:
  double squared_deviation_from_mean();
  inline bool check_cache_flags(uint8_t chosen_flags);

  static double interpolate_quantile(const std::vector<T>& data, double h);
  static double r1(const std::vector<T>& data, double p);
  static double r2(const std::vector<T>& data, double p);
  static double r3(const std::vector<T>& data, double p);
  static double r4(const std::vector<T>& data, double p);
  static double r5(const std::vector<T>& data, double p);
  static double r6(const std::vector<T>& data, double p);
  static double r7(const std::vector<T>& data, double p);
  static double r8(const std::vector<T>& data, double p);
  static double r9(const std::vector<T>& data, double p);

  typedef double (*quantile_func)(const std::vector<T>&, double);

  static constexpr quantile_func quantile_function_map[] = {
      &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9};

  /**< The default quantile algorithm used by future Stats1D objects*/
  QuantileAlgorithm default_quantile_alg;

  constexpr static uint8_t SORTED = 2;
  constexpr static uint8_t MEAN = 4;
  constexpr static uint8_t SUMSQ = 8;
  constexpr static uint8_t IQR = 16;
  constexpr static uint8_t FIVENUM = 32;

  std::vector<T> original_data;
  std::vector<T> sorted_data;
  // sorted, mean, variance, iqr, fivenum
  // 0b00011111
  uint8_t cache_flags;
  double mean_, stdev_, sum_sq_, iqr_;
  std::vector<T> modes;

  Summary fivenum;
};

template <typename T>
template <typename ForwardIter>
Stats1D<T>::Stats1D(ForwardIter a, const ForwardIter b, bool sorted)
    : original_data(a, b),
      sorted_data(a, b),
      cache_flags(SORTED),
      default_quantile_alg(QuantileAlgorithm::R6) {
  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
  }
}

template <typename T>
template <typename Iterable>
Stats1D<T>::Stats1D(const Iterable& container, bool sorted)
    : Stats1D(std::begin(container), std::end(container), sorted) {}

template <typename T>
template <typename Iterable>
void Stats1D<T>::update_data(const Iterable& container, bool sorted) {
  updateData(std::begin(container), std::end(container), sorted);
}

template <typename T>
double Stats1D<T>::mean() {
  if (check_cache_flags(MEAN)) return mean_;

  cache_flags |= MEAN;

  double mean_tmp = 0;
  for (auto const& num : sorted_data) {
    mean_tmp += num;
  }

  mean_tmp /= sorted_data.size();
  return mean_ = mean_tmp;
}

template <typename T>
std::vector<T> Stats1D<T>::multimode() {
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

template <typename T>
double Stats1D<T>::iqr() {
  if (check_cache_flags(IQR)) return iqr_;

  cache_flags |= IQR;

  Summary fn = five_number_summary();
  return iqr_ = fn.q3 - fn.q1;
}

template <typename T>
Summary Stats1D<T>::five_number_summary() {
  if (check_cache_flags(FIVENUM)) return fivenum;

  cache_flags |= FIVENUM;

  struct Summary fn;
  fn.min = *sorted_data.begin();
  fn.max = *(sorted_data.end() - 1);

  fn.q1 = quantile(.25, default_quantile_alg);
  fn.median = quantile(.50, default_quantile_alg);
  fn.q3 = quantile(.75, default_quantile_alg);

  fivenum = Summary(fn);

  return fivenum;
}

template <typename T>
double Stats1D<T>::stdev() {
  return sqrt(variance());
}

template <typename T>
double Stats1D<T>::pstdev() {
  return sqrt(pvariance());
}

template <typename T>
double Stats1D<T>::squared_deviation_from_mean() {
  if (check_cache_flags(SUMSQ)) return sum_sq_;

  double mean_tmp = mean();
  double sum = 0;
  for (auto const& num : sorted_data) {
    sum += pow(num - mean_tmp, 2);
  }

  return sum_sq_ = sum;
}

template <typename T>
bool Stats1D<T>::check_cache_flags(uint8_t chosen_flags) {
  return (cache_flags & chosen_flags) == chosen_flags;
}

template <typename T>
double Stats1D<T>::variance() {
  return squared_deviation_from_mean() / (sorted_data.size() - 1);
}

template <typename T>
double Stats1D<T>::pvariance() {
  return squared_deviation_from_mean() / sorted_data.size();
}

template <typename T>
double Stats1D<T>::quantile(double percentile, QuantileAlgorithm alg) {
  return (Stats1D<T>::quantile_function_map[(int)alg])(sorted_data, percentile);
}

template <typename T>
void Stats1D<T>::set_quantile_alg(QuantileAlgorithm q) {
  default_quantile_alg = q;
}

// template <typename U, typename T>
// double expected_xy(Stats1D<T>& stats1, Stats1D<U>& stats2) {
//   double sum_xy = 0;
//   for (int i = 0; i < stats1.size(); i++) {
//     sum_xy += stats1[i] * stats2[i];
//   }
//   return sum_xy / stats1.size();
// }

// template <typename U, typename T>
// double covariance(Stats1D<T>& stats1, Stats1D<U>& stats2) {
//   // TODO: throw exception if not same length VECTOR_MIS...

//   double sum = 0;

//   for (int i = 0; i < stats1.size(); i++) {
//     sum += (stats1[i] - stats1.getMean()) * (stats2[i] - stats2.getMean());
//   }
//   // std::cout << sum / stats1.size() << std::endl;
//   return sum / (stats1.size() - 1);
// }

// template <typename U, typename T>
// double pearson_correlation(Stats1D<T>& stats1, Stats1D<U>& stats2) {
//   return covariance(stats1, stats2) / (stats1.getStdDev() *
//   stats2.getStdDev());
// }

template <typename T>
std::ostream& operator<<(std::ostream& os, Stats1D<T>& stats) {
  struct Summary fivenum = stats.five_number_summary();
  os << "# Points: " << stats.sorted_data.size() << "\nMean: " << stats.mean()
     << "\nSample StdDev: " << stats.stdev()
     << "\nPopulation StdDev: " << stats.pstdev()
     << "\nSample Variance: " << stats.variance()
     << "\nPopulation Variance: " << stats.pvariance()
     << "\nFive Number Summary:\n\tMinimum: " << fivenum.min
     << "\n\tFirst Quartile: " << fivenum.q1 << "\n\tMedian: " << fivenum.median
     << "\n\tThird Quartile: " << fivenum.q3 << "\n\tMaximum: " << fivenum.max
     << "\nIQR: " << stats.iqr() << "\nModes: [";

  std::vector<T> modes = stats.multimode();
  for (auto const& i : modes) {
    os << i << ", ";
  }
  os << "]";
  return os;
}

template <typename T>
double Stats1D<T>::r1(const std::vector<T>& data, double p) {
  double h = data.size() * p + 0.5;
  return data[ceil(h - 0.5) - 1];
}

template <typename T>
double Stats1D<T>::r2(const std::vector<T>& data, double p) {
  double h = data.size() * p + 0.5;
  return (data[ceil(h - 0.5) - 1] + data[floor(h + 0.5) - 1]) / 2;
}

template <typename T>
double Stats1D<T>::r3(const std::vector<T>& data, double p) {
  double h = data.size() * p;
  return data[round(h) - 1];
}

template <typename T>
double Stats1D<T>::interpolate_quantile(const std::vector<T>& data, double h) {
  return data[floor(h) - 1] +
         (h - floor(h)) * (data[ceil(h) - 1] - data[floor(h) - 1]);
}

template <typename T>
double Stats1D<T>::r4(const std::vector<T>& data, double p) {
  double h = data.size() * p;
  return interpolate_quantile(data, h);
}

template <typename T>
double Stats1D<T>::r5(const std::vector<T>& data, double p) {
  double h = data.size() * p + 0.5;
  return interpolate_quantile(data, h);
}

template <typename T>
double Stats1D<T>::r6(const std::vector<T>& data, double p) {
  double h = (data.size() + 1) * p;
  return interpolate_quantile(data, h);
}

template <typename T>
double Stats1D<T>::r7(const std::vector<T>& data, double p) {
  double h = (data.size() - 1) * p + 1;
  return interpolate_quantile(data, h);
}

template <typename T>
double Stats1D<T>::r8(const std::vector<T>& data, double p) {
  double h = (data.size() + 1 / 3) * p + 1 / 3;
  return interpolate_quantile(data, h);
}

template <typename T>
double Stats1D<T>::r9(const std::vector<T>& data, double p) {
  double h = (data.size() + 0.25) * p + .375;
  return interpolate_quantile(data, h);
}

}  // namespace stats
