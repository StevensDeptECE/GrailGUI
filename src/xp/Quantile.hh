#pragma once

#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T>
using quantile_func = std::function<double(std::vector<T>, double)>;

/**
 * */
template <typename T>
double r1(std::vector<T> data, double p) {
  double h = data.size() * p + 0.5;
  return data[ceil(h - 0.5) - 1];
}

template <typename T>
double r2(std::vector<T> data, double p) {
  double h = data.size() * p + 0.5;
  return (data[ceil(h - 0.5) - 1] + data[floor(h + 0.5) - 1]) / 2;
}

template <typename T>
double r3(std::vector<T> data, double p) {
  double h = data.size() * p;
  return data[round(h) - 1];
}

template <typename T>
double interpolate_quantile(std::vector<T> data, double h) {
  return data[floor(h) - 1] +
         (h - floor(h)) * (data[ceil(h) - 1] - data[floor(h) - 1]);
}

template <typename T>
double r4(std::vector<T> data, double p) {
  double h = data.size() * p;
  return interpolate_quantile(data, h);
}

template <typename T>
double r5(std::vector<T> data, double p) {
  double h = data.size() * p + 0.5;
  return interpolate_quantile(data, h);
}

template <typename T>
double r6(std::vector<T> data, double p) {
  double h = (data.size() + 1) * p;
  return interpolate_quantile(data, h);
}

template <typename T>
double r7(std::vector<T> data, double p) {
  double h = (data.size() - 1) * p + 1;
  return interpolate_quantile(data, h);
}

template <typename T>
double r8(std::vector<T> data, double p) {
  double h = (data.size() + 1 / 3) * p + 1 / 3;
  return interpolate_quantile(data, h);
}

template <typename T>
double r9(std::vector<T> data, double p) {
  double h = (data.size() + 0.25) * p + .375;
  return interpolate_quantile(data, h);
}

template <typename T>
void init_quantile_algs(std::unordered_map<std::string, quantile_func<T>>* qa) {
  using namespace std::placeholders;
  qa->insert_or_assign("R-1", std::bind(r1<T>, _1, _2));
  qa->insert_or_assign("R-2", std::bind(r2<T>, _1, _2));
  qa->insert_or_assign("R-3", std::bind(r3<T>, _1, _2));
  qa->insert_or_assign("R-4", std::bind(r4<T>, _1, _2));
  qa->insert_or_assign("R-5", std::bind(r5<T>, _1, _2));
  qa->insert_or_assign("R-6", std::bind(r6<T>, _1, _2));
  qa->insert_or_assign("R-7", std::bind(r7<T>, _1, _2));
  qa->insert_or_assign("R-8", std::bind(r8<T>, _1, _2));
  qa->insert_or_assign("R-9", std::bind(r9<T>, _1, _2));
}
