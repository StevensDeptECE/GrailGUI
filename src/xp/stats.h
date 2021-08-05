#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <unordered_map>
#include <vector>

namespace stats {

struct Summary {
  double min, max, q1, q3, median;
};

enum class QuantileAlgorithm { R1 = 0, R2, R3, R4, R5, R6, R7, R8, R9 };

template <typename Iterable>
double mean(const Iterable& data) {
  double mean_tmp = 0;
  for (auto const& num : data) {
    mean_tmp += num;
  }

  return mean_tmp /= std::size(data);
}

template <typename Iterable>
std::vector<double> multimode(const Iterable& data) {
  std::unordered_map<double, int> map;
  std::vector<double> modes;
  double biggest_mode = 1;

  for (auto const& num : data) {
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

template <typename Iterable>
double squared_deviation_from_mean(const Iterable& data, double mu) {
  double sum = 0;
  for (auto const& num : data) {
    sum += pow(num - mu, 2);
  }

  return sum;
}

template <typename Iterable>
double variance(const Iterable& data, std::optional<double> mu = {}) {
  return squared_deviation_from_mean(data, mu.value_or(mean(data))) /
         (std::size(data) - 1);
}

template <typename Iterable>
double pvariance(const Iterable& data, std::optional<double> mu = {}) {
  return squared_deviation_from_mean(data, mu.value_or(mean(data))) /
         (std::size(data));
}

template <typename Iterable>
double stdev(const Iterable& data, std::optional<double> xbar = {}) {
  double mu = xbar.value_or(mean(data));
  return sqrt(variance(data, mu));
}

template <typename Iterable>
double pstdev(const Iterable& data, std::optional<double> xbar = {}) {
  double mu = xbar.value_or(mean(data));
  return sqrt(pvariance(data, mu));
}

namespace {

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
double interpolate_quantile(const Iterable& data, double h) {
  return data[floor(h) - 1] +
         (h - floor(h)) * (data[ceil(h) - 1] - data[floor(h) - 1]);
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
}  // namespace

template <typename Iterable>
double quantile(const Iterable& data, double percentile,
                QuantileAlgorithm alg = QuantileAlgorithm::R6) {
  std::vector<double> sorted(std::begin(data), std::end(data));
  sort(sorted.begin(), sorted.end());

  double val;
  switch (alg) {
    case QuantileAlgorithm::R1:
      val = r1(sorted, percentile);
      break;
    case QuantileAlgorithm::R2:
      val = r2(sorted, percentile);
      break;
    case QuantileAlgorithm::R3:
      val = r3(sorted, percentile);
      break;
    case QuantileAlgorithm::R4:
      val = r4(sorted, percentile);
      break;
    case QuantileAlgorithm::R5:
      val = r5(sorted, percentile);
      break;
    case QuantileAlgorithm::R6:
      val = r6(sorted, percentile);
      break;
    case QuantileAlgorithm::R7:
      val = r7(sorted, percentile);
      break;
    case QuantileAlgorithm::R8:
      val = r8(sorted, percentile);
      break;
    case QuantileAlgorithm::R9:
      val = r9(sorted, percentile);
      break;
  }

  return val;
}

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

}  // namespace stats
