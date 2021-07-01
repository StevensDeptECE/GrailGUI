#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

template <typename T>
class Stats1D {
 public:
  struct Summary {
    double min, max, q1, q3, median;
  };

 private:
  T* array;
  std::vector<T> sorted_array;
  uint32_t size;
  bool sorted;
  std::unique_ptr<double> mean, stddev, variance;
  std::unique_ptr<T> iqr;
  std::vector<T> modes;

  std::unique_ptr<struct Summary> fivenum;
  uint32_t getMedian(uint32_t left, uint32_t right);

 public:
  Stats1D() = delete;
  Stats1D(T* array, uint32_t size, bool sorted = false);
  void updateArray(T* newArray, uint32_t newSize, bool sorted = false);
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

/**
 * @brief Construct a new Stats1D object of a numeric type
 *
 * The array may be of any numeric type and may be unsorted when it is passed
 * into the object. The size of the array is assumed to match the size of the
 * variable. The sorted flag defaults to false.
 *
 * If a mistake is made when creating the array, or the array is updated, use
 * updateArray to make any necessary changes.
 *
 * @tparam T Any numeric type
 * @param array A pointer to the array to be analyzed
 * @param size The size of the array to be analyzed
 * @param sorted A flag indicating if the array is sorted
 */
template <typename T>
Stats1D<T>::Stats1D(T* array, uint32_t size, bool sorted)
    : array(array), size(size) {
  sorted_array = vector<T>(array, (array + size));
  if (!sorted) {
    sort(sorted_array.begin(), sorted_array.end());
    sorted = true;
  }
}

/**
 * @brief Updates the content of the array
 *
 * @tparam T Any numeric type
 * @param newArray A new array
 * @param newSize A new size
 * @param sorted A new sorted flag, defaults to the existing flag
 */
template <typename T>
void Stats1D<T>::updateArray(T* newArray, uint32_t newSize, bool sorted) {
  this->array = array;
  this->size = size;
  this->sorted = sorted;
  if (!sorted) {
    sort(sorted_array.begin(), sorted_array.end());
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
  for (int i = 0; i < size; i++) {
    mean_tmp += array[i];
  }
  mean_tmp /= size;

  mean = make_unique<double>(mean_tmp);
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
vector<T> Stats1D<T>::getModes() {
  if (modes.size() != 0 || size == 0) return modes;
  if (!sorted) {
    sort(sorted_array.begin(), sorted_array.end());
    sorted = !sorted;
  }

  T curMode;
  uint32_t curOcc, maxOcc;

  for (int i = 0; i < size; i++) {
    curOcc = (curMode == array[i]) ? (++curOcc) : curOcc;
    if (maxOcc < curOcc && curOcc != 1) {
      maxOcc = curOcc;
      curMode = array[i];
      modes.clear();
      modes.push_back(array[i]);
    } else if (maxOcc == curOcc) {
      modes.push_back(array[i]);
    }
  }

  return modes;
}

/**
 * @brief Calculates the IQR of a numeric type
 *
 * The interquartile range is the 50% range between the first and third quartile
 * of a dataset. Together with the median of the dataset, it presents an
 * alternative to the mean and standard deviation for finding outliers.
 *
 * @tparam T Any numeric type
 * @return T The IQR of a dataset
 */
template <typename T>
T Stats1D<T>::getIQR() {
  if (iqr) return *iqr.get();
  if (!sorted) {
    sort(sorted_array.begin(), sorted_array.end());
    sorted = !sorted;
  }
  Stats1D<T>::getSummary();
  struct Summary fn = *fivenum.get();
  iqr = make_unique<T>(fn.q3 - fn.q1);
  return *iqr.get();
}

/**
 * @brief Returns a struct containing the five number summary of a numeric type
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
    sort(sorted_array.begin(), sorted_array.end());
    sorted = !sorted;
  }
  struct Summary fn;
  fn.min = sorted_array[0];
  fn.max = sorted_array[size - 1];

  // double q1_ind = .25 * size;
  // double mid_ind = .5 * size;
  // double q3_ind = .75 * size;
  // double epsilon = 1 / 10000;
  // if (fmod(q1_ind, 1) > epsilon)
  //   fn.q1 = (array[static_cast<uint32_t>(floor(q1_ind))] +
  //            array[static_cast<uint32_t>(ceil(q1_ind))]) /
  //           2.0;
  // else
  //   fn.q1 = array[static_cast<uint32_t>(q1_ind)];

  // if (fmod(mid_ind, 1) > epsilon)
  //   fn.median = (array[static_cast<uint32_t>(floor(mid_ind))] +
  //                array[static_cast<uint32_t>(ceil(mid_ind))]) /
  //               2.0;
  // else
  //   fn.median = array[static_cast<uint32_t>(mid_ind)];

  // if (fmod(q3_ind, 1) > epsilon)
  //   fn.q3 = (array[static_cast<uint32_t>(floor(q3_ind))] +
  //            array[static_cast<uint32_t>(ceil(q3_ind))]) /
  //           2.0;
  // else
  //   fn.q3 = array[static_cast<uint32_t>(q3_ind)];

  fn.q1 = getQuantile(.25);
  fn.median = getQuantile(.50);
  fn.q3 = getQuantile(.75);

  fivenum = make_unique<struct Summary>(fn);
  return *fivenum.get();
}

/**
 * @brief Calculates the standard deviation of a numeric type
 *
 * The standard deviation of a dataset describes the spread of a data. A higher
 * standard deviation indicates that the data is spread further from the mean of
 * the dataset.
 *
 * @tparam T Any numeric type
 * @return double The standard deviation of a dataset
 */
template <typename T>
double Stats1D<T>::getStdDev() {
  if (stddev) return *stddev.get();
  stddev = make_unique<double>(sqrt(getVariance()));
  return *stddev.get();
}

/**
 * @brief Calculates the variance of a numeric type
 *
 * The variance of a dataset is the square of standard deviation and is another
 * descriptor of the spread of a dataset. Among its many uses are sampling,
 * inference, hypothesis testing, and goodness of fit.
 *
 * @tparam T Any numeric type
 * @return double The variance of a dataset
 */
template <typename T>
double Stats1D<T>::getVariance() {
  if (variance) return *variance.get();
  double mean_tmp = getMean();
  double sum = 0;
  for (int i = 0; i < size; i++) {
    sum += pow(array[i] - mean_tmp, 2);
  }
  variance = make_unique<double>(sum / (size - 1));
  return *variance.get();
}

/**
 * @brief getQuantile - Gets a quantile of the sorted array
 *
 * @param percentile The percentile to look for
 * @return double The resultant quantile
 **/
template <typename T>
double Stats1D<T>::getQuantile(double percentile) {
  double h = (sorted_array.size() - 1) * percentile + 1;
  return sorted_array[floor(h)] +
         (h - floor(h)) * (sorted_array[ceil(h)] - sorted_array(floor(h)));
}

template <typename T>
ostream& operator<<(ostream& os, Stats1D<T>& stats) {
  struct Stats1D<T>::Summary fivenum = stats.getSummary();
  os << "Mean: " << stats.getMean() << "\nStdDev: " << stats.getStdDev()
     << "\nVariance: " << stats.getVariance()
     << "\nFive Number Summary:\n\tMinimum: " << fivenum.min
     << "\n\tFirst Quartile: " << fivenum.q1 << "\n\tMedian: " << fivenum.median
     << "\n\tThird Quartile: " << fivenum.q3 << "\n\tMaximum: " << fivenum.max
     << "\nIQR:" << stats.getIQR() << "\nModes: [";

  vector<T> modes = stats.getModes();
  for (const auto& i : modes) {
    os << i << ", ";
  }
  os << "]";
  return os;
}
