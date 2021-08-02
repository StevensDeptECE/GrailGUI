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
  template <typename FowardIter>
  Stats1D(FowardIter a, const FowardIter b, bool sorted = false);

  template <typename Iterable>
  Stats1D(const Iterable& container, bool sorted = false);

  template <typename FowardIter>
  void updateData(FowardIter a, const FowardIter b, bool sorted = false);

  template <typename Iterable>
  void updateData(const Iterable& container, bool sorted = false);

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
  double getMean();

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
  std::vector<T> getModes();

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
  double getIQR();

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
  Summary getSummary();

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
  double getSampleStdDev();

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
  double getPopulationStdDev();

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
  double getSampleVariance();

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
  double getPopulationVariance();

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
  double getQuantile(double percentile, QuantileAlgorithm alg);

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
  void setQuantileAlgorithm(QuantileAlgorithm alg);

  T operator[](int i) const { return original_data[i]; }

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os, Stats1D<U>& stats);

 private:
  static double r1(const std::vector<T>& data, double p);
  static double r2(const std::vector<T>& data, double p);
  static double r3(const std::vector<T>& data, double p);
  static double interpolate_quantile(const std::vector<T>& data, double h);
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
  QuantileAlgorithm defaultQuantile;

  constexpr static uint8_t SORTED = 0b10000000;
  constexpr static uint8_t MEAN = 0b01000000;
  constexpr static uint8_t POPSTDDEV = 0b00100000;
  constexpr static uint8_t POPVARIANCE = 0b00010000;
  constexpr static uint8_t SAMPSTDDEV = 0b00001000;
  constexpr static uint8_t SAMPVARIANCE = 0b00000100;
  constexpr static uint8_t IQR = 0b00000010;
  constexpr static uint8_t FIVENUM = 0b00000001;

  std::vector<T> original_data;
  std::vector<T> sorted_data;
  // sorted, mean, population stddev, population variance,
  // sample stddev, sample variance, iqr, fivenum
  // 0b11111111
  uint8_t cache_flags;
  double mean, samp_stddev, samp_variance, pop_stddev, pop_variance, iqr;
  std::vector<T> modes;

  Summary fivenum;
  QuantileAlgorithm quantile = defaultQuantile;
};

template <typename T>
template <typename ForwardIter>
Stats1D<T>::Stats1D(ForwardIter a, const ForwardIter b, bool sorted)
    : original_data(a, b),
      sorted_data(a, b),
      cache_flags(SORTED),
      defaultQuantile(QuantileAlgorithm::R6) {
  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
  }
}

template <typename T>
template <typename Iterable>
Stats1D<T>::Stats1D(const Iterable& container, bool sorted)
    : Stats1D(std::begin(container), std::end(container), sorted) {}

template <typename T>
template <typename FowardIter>
void Stats1D<T>::updateData(FowardIter a, const FowardIter b, bool sorted) {
  original_data(a, b);
  sorted_data(a, b);

  if (!sorted) {
    sort(sorted_data.begin(), sorted_data.end());
  }

  cache_flags = SORTED;

  modes.clear();
}

template <typename T>
template <typename Iterable>
void Stats1D<T>::updateData(const Iterable& container, bool sorted) {
  updateData(std::begin(container), std::end(container), sorted);
}

template <typename T>
double Stats1D<T>::getMean() {
  if ((cache_flags & MEAN) == MEAN) return mean;

  double mean_tmp = 0;

  for (auto const& num : sorted_data) {
    mean_tmp += num;
  }

  mean_tmp /= sorted_data.size();

  mean = mean_tmp;

  cache_flags |= MEAN;

  return mean;
}

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

template <typename T>
double Stats1D<T>::getIQR() {
  if ((cache_flags & IQR) == IQR) return iqr;

  Summary fn = getSummary();
  iqr = fn.q3 - fn.q1;
  return iqr;
}

template <typename T>
Summary Stats1D<T>::getSummary() {
  if ((cache_flags & FIVENUM) == FIVENUM) return fivenum;

  struct Summary fn;
  fn.min = *sorted_data.begin();
  fn.max = *(sorted_data.end() - 1);

  fn.q1 = getQuantile(.25, defaultQuantile);
  fn.median = getQuantile(.50, defaultQuantile);
  fn.q3 = getQuantile(.75, defaultQuantile);

  fivenum = Summary(fn);

  cache_flags |= FIVENUM;

  return fivenum;
}

template <typename T>
double Stats1D<T>::getSampleStdDev() {
  if ((cache_flags & SAMPSTDDEV) == SAMPSTDDEV) return samp_stddev;

  samp_stddev = sqrt(getSampleVariance());
  cache_flags |= SAMPSTDDEV;
  return samp_stddev;
}

template <typename T>
double Stats1D<T>::getPopulationStdDev() {
  if ((cache_flags & POPSTDDEV) == POPSTDDEV) return pop_stddev;

  pop_stddev = sqrt(getPopulationVariance());
  cache_flags |= POPSTDDEV;
  return pop_stddev;
}

template <typename T>
double Stats1D<T>::getSampleVariance() {
  if ((cache_flags & SAMPVARIANCE) == SAMPVARIANCE) return samp_variance;

  double mean_tmp = getMean();
  double sum = 0;
  for (auto const& num : sorted_data) {
    sum += pow(num - mean_tmp, 2);
  }
  samp_variance = sum / (sorted_data.size() - 1);
  cache_flags |= SAMPVARIANCE;
  return samp_variance;
}

template <typename T>
double Stats1D<T>::getPopulationVariance() {
  if ((cache_flags & POPVARIANCE) == POPVARIANCE) return pop_variance;

  double mean_tmp = getMean();
  double sum = 0;
  for (auto const& num : sorted_data) {
    sum += pow(num - mean_tmp, 2);
  }
  pop_variance = sum / (sorted_data.size());
  cache_flags |= POPVARIANCE;
  return pop_variance;
}

template <typename T>
double Stats1D<T>::getQuantile(double percentile, QuantileAlgorithm alg) {
  return (Stats1D<T>::quantile_function_map[(int)alg])(sorted_data, percentile);
}

template <typename T>
void Stats1D<T>::setQuantileAlgorithm(QuantileAlgorithm q) {
  defaultQuantile = q;
}

template <typename U, typename T>
double expected_xy(Stats1D<T>& stats1, Stats1D<U>& stats2) {
  double sum_xy = 0;
  for (int i = 0; i < stats1.size(); i++) {
    sum_xy += stats1[i] * stats2[i];
  }
  return sum_xy / stats1.size();
}

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
  struct Summary fivenum = stats.getSummary();
  os << "# Points: " << stats.sorted_data.size()
     << "\nMean: " << stats.getMean()
     << "\nSample StdDev: " << stats.getSampleStdDev()
     << "\nPopulation StdDev: " << stats.getPopulationStdDev()
     << "\nSample Variance: " << stats.getSampleVariance()
     << "\nPopulation Variance: " << stats.getPopulationVariance()
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
