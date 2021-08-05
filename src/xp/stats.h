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

}  // namespace stats
