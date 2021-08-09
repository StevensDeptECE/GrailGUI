#pragma once
#ifndef STATS_H_
#define STATS_H_

#include <algorithm>
#include <cmath>
#include <optional>

/**
 * A library providing functions for calculating mathematical statsitics of
 * numeric (not complex/imaginary) data. This is not meant to be a replacement
 * to massive machine learning libraries like dlib or CUDA, just something small
 * to include with projects that need basic statsistics.
 *
 * All of these functions are designed to take in a generic iterable of some
 * kind (arrays, vectors, etc) and return a double precison floating point
 * number.
 */
namespace stats {

namespace detail {

template <typename Iterable>
double squared_deviation_from_mean(const Iterable& data, double mu) {
  double sum = 0;
  for (auto const& num : data) {
    sum += pow(num - mu, 2);
  }

  return sum;
}

template <typename Iterable>
double interpolate_quantile(const Iterable& data, double h) {
  return data[floor(h) - 1] +
         (h - floor(h)) * (data[ceil(h) - 1] - data[floor(h) - 1]);
}

template <typename Iterable>
double r1(const Iterable& data, double p) {
  double h = data.size() * p + 0.5;
  return data[ceil(h - 0.5) - 1];
}

template <typename Iterable>
double r2(const Iterable& data, double p) {
  double h = data.size() * p + 0.5;
  return (data[ceil(h - 0.5) - 1] + data[floor(h + 0.5) - 1]) / 2;
}

template <typename Iterable>
double r3(const Iterable& data, double p) {
  double h = data.size() * p;
  return data[round(h) - 1];
}

template <typename Iterable>
double r4(const Iterable& data, double p) {
  double h = data.size() * p;
  return interpolate_quantile(data, h);
}

template <typename Iterable>
double r5(const Iterable& data, double p) {
  double h = data.size() * p + 0.5;
  return interpolate_quantile(data, h);
}

template <typename Iterable>
double r6(const Iterable& data, double p) {
  double h = (data.size() + 1) * p;
  return interpolate_quantile(data, h);
}

template <typename Iterable>
double r7(const Iterable& data, double p) {
  double h = (data.size() - 1) * p + 1;
  return interpolate_quantile(data, h);
}

template <typename Iterable>
double r8(const Iterable& data, double p) {
  double h = (data.size() + 1 / 3) * p + 1 / 3;
  return interpolate_quantile(data, h);
}

template <typename Iterable>
double r9(const Iterable& data, double p) {
  double h = (data.size() + 0.25) * p + .375;
  return interpolate_quantile(data, h);
}
}  // namespace detail

struct Summary {
  double min;    /** */
  double max;    /** */
  double q1;     /** */
  double q3;     /** */
  double median; /** */
};

enum class QuantileAlgorithm { R1 = 0, R2, R3, R4, R5, R6, R7, R8, R9 };

/**
 * @brief Returns the sample arithmetic mean of data.
 *
 * The arithmetic mean is the sum of the data divided by the number of data
 * points. Often it's refered to as "the average", but it's only one of many
 * different mathematical averages.
 *
 * TODO: implement this
 * If data is empty, an exception should be thrown (not implemented yet).
 *
 * @tparam Iterable
 * @param data an iterable of numbers.
 * @return double the mean of data.
 */
template <typename Iterable>
double mean(const Iterable& data) {
  double mean_tmp = 0;
  for (auto const& num : data) {
    mean_tmp += num;
  }

  return mean_tmp /= std::size(data);
}

template <typename Iterable>
double median(const Iterable& data) {
  std::vector<double> sorted(std::begin(data), std::end(data));
  sort(sorted.begin(), sorted.end());
  int n = sorted.size();

  if (n % 2 == 1) {
    return sorted[n / 2];

  } else {
    int i = n / 2;
    return (sorted[i - 1] + sorted[i]) / 2;
  }
}

/**
 * @brief Returns a vector of the most frequently occuring values.
 *
 * This will return multiple values if there are multiple modes, and values
 * will be in the order they were first encountered in the data.
 *
 * TODO: add a check for this
 * If data is empty an empty vector will be returned.
 *
 * @tparam Iterable
 * @param data an iterable of numbers.
 * @return std::vector<double> the modes of the data.
 */
template <typename Iterable>
std::vector<double> multimode(const Iterable& data) {
  std::unordered_map<double, int> map;
  std::vector<double> modes;
  double biggest_mode = 1;

  for (auto const& num : data) {
    if (map.find(num) == map.end()) {
      map[num] = 1;
    } else {
      map[num] += 1;
      if (map[num] > biggest_mode) biggest_mode += 1;
    }
  }

  for (auto const& [key, value] : map) {
    if (value == biggest_mode) {
      modes.push_back(key);
    }
  }

  return modes;
}

template <typename Iterable>
double mode(const Iterable& data) {
  std::vector<double> modes = multimode(data);
  if (modes.size()) return multimode(data)[0];
  // TODO: implement exception to throw when we have zero length thing
}

/**
 * @brief Returns the sample variance of data.
 *
 * Variance, also called the second moment about the mean, is a measure of the
 * variability / spread of data. A large variance indicates that the data is
 * spread out, and a small variance indicates it is clustered closely around the
 * mean.
 *
 * If the optional second argument xbar is given, it should be the mean of the
 * data. If it is missing or {} (the default), the mean is automatically
 * calculated.
 *
 * TODO: make the reference to pvariance jump there in documentation (if it
 * doesn't already)
 * Use this function when your data is a sample from a
 * population. To calculate the variance from the entire population, see
 * pvariance().
 *
 * TODO: implement this exception
 * If data contains less than two values, an exception should be thrown.
 *
 * @tparam Iterable
 * @param data an iterable of at least two real-valued numbers.
 * @param xbar the mean of the data.
 * @return double the variance of the data.
 */
template <typename Iterable>
double variance(const Iterable& data, std::optional<double> xbar = {}) {
  return detail::squared_deviation_from_mean(data, xbar.value_or(mean(data))) /
         (std::size(data) - 1);
}

/**
 * @brief Returns the population variance of the data.
 *
 * Variance, also called the second moment about the mean, is a measure of the
 * variability / spread of data. A large variance indicates that the data is
 * spread out, and a small variance indicates it is clustered closely around the
 * mean.
 *
 * If the optional second argument @p mu is given, it is typically the mean of
 * the @p data. It can also be used to compute the second moment around a point
 * that is not the mean. If it is missing or {} (the default), the arithmetic
 * mean is automatically calculated.
 *
 * Use this functio to calculate the variance from the entire population, to
 * estimate the variance from a sample, the variance() function is usually a
 * better choice.
 *
 * If data contains less than two values, an exception should be thrown.
 *
 * TODO: show an example here and for other functions
 * \b Usage:
 * \code
 *
 * \endcoe
 *
 * @tparam Iterable
 * @param data an iterable of at least two real-valued numbers.
 * @param mu where to calculate the second moment around.
 * @return double the population variance of the data.
 */
template <typename Iterable>
double pvariance(const Iterable& data, std::optional<double> mu = {}) {
  return detail::squared_deviation_from_mean(data, mu.value_or(mean(data))) /
         (std::size(data));
}

/**
 * @brief
 *
 * @tparam Iterable
 * @param data
 * @param xbar
 * @return double
 */
template <typename Iterable>
double stdev(const Iterable& data, std::optional<double> xbar = {}) {
  return sqrt(variance(data, xbar.value_or(mean(data))));
}

/**
 * @brief
 *
 * @tparam Iterable
 * @param data
 * @param mu
 * @return double
 */
template <typename Iterable>
double pstdev(const Iterable& data, std::optional<double> mu = {}) {
  return sqrt(pvariance(data, mu.value_or(mean(data))));
}

/**
 * @brief
 *
 * @tparam Iterable
 * @param data
 * @param percentile
 * @param alg
 * @return double
 */
template <typename Iterable>
double quantile(const Iterable& data, double percentile,
                QuantileAlgorithm alg = QuantileAlgorithm::R6) {
  std::vector<double> sorted(std::begin(data), std::end(data));
  sort(sorted.begin(), sorted.end());

  double val;
  switch (alg) {
    case QuantileAlgorithm::R1:
      val = detail::r1(sorted, percentile);
      break;
    case QuantileAlgorithm::R2:
      val = detail::r2(sorted, percentile);
      break;
    case QuantileAlgorithm::R3:
      val = detail::r3(sorted, percentile);
      break;
    case QuantileAlgorithm::R4:
      val = detail::r4(sorted, percentile);
      break;
    case QuantileAlgorithm::R5:
      val = detail::r5(sorted, percentile);
      break;
    case QuantileAlgorithm::R6:
      val = detail::r6(sorted, percentile);
      break;
    case QuantileAlgorithm::R7:
      val = detail::r7(sorted, percentile);
      break;
    case QuantileAlgorithm::R8:
      val = detail::r8(sorted, percentile);
      break;
    case QuantileAlgorithm::R9:
      val = detail::r9(sorted, percentile);
      break;
  }

  return val;
}

/**
 * @brief
 *
 * @tparam Iterable
 * @param data
 * @param alg
 * @return Summary
 */
template <typename Iterable>
Summary five_number_summary(const Iterable& data,
                            QuantileAlgorithm alg = QuantileAlgorithm::R6) {
  std::vector<double> sorted(std::begin(data), std::end(data));
  sort(sorted.begin(), sorted.end());

  Summary fn = {.min = *sorted.begin(),
                .max = *(sorted.end() - 1),
                .q1 = quantile(sorted, .25, alg),
                .q3 = quantile(sorted, .75, alg),
                .median = quantile(sorted, .50, alg)};

  return fn;
}

// TODO: does this really need to exist if we have the five number summary?
// could we just make the five number sum be more descriptive with its fields
// (equivalent to one var stats on a ti calculator?)
/**
 * @brief
 *
 * @tparam Iterable
 * @param data
 * @param alg
 * @return double
 */
template <typename Iterable>
double iqr(const Iterable& data,
           QuantileAlgorithm alg = QuantileAlgorithm::R6) {
  std::vector<double> sorted(std::begin(data), std::end(data));
  sort(sorted.begin(), sorted.end());

  return quantile(sorted, .75, alg) - quantile(sorted, .25, alg);
}

}  // namespace stats

#endif  // STATS_H_
