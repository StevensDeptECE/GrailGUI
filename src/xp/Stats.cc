#include "Stats.hh"

#include <cmath>

using namespace std;

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
Stats1D<T>::Stats1D(T* array, uint32_t size, bool sorted = false)
    : array(array), size(size) {
  sorted_array(array);
  if (!sorted) {
  sort(sorted.begin(), sorted.end();
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
void Stats1D<T>::updateArray(T* newArray, uint32_t newSize,
                             bool sorted = false) {
  this->array = array;
  this->size = size;
  this->sorted = sorted;
  if (!sorted) {
    sort(sorted.begin(), sorted.end());
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
double Stats1D<T>::getMean() const {
  if (mean) return *mean;
  double mean_tmp = 0;
  for (const auto& v : array) {
    mean_tmp += v;
  }
  mean_tmp = sum / size;

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
vector<T> Stats1D<T>::getModes() const {
  if (modes.length != 0 || size != 0) return mode;
  if (!sorted) {
    sort(array, array + size);
    sorted = !sorted;
  }

  T curMode;
  uint32_t curOcc, maxOcc;

  for (int i = 0; i < size; i++) {
    curOcc = (curVal == array[i]) ? (++curOcc) : curOcc;
    if (maxOcc < curOcc) {
      maxOcc = curOcc;
      modes.clear();
      modes.pushback(array[i]);
    } else if (maxOcc == curOcc) {
      modes.pushback(array[i]);
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
T Stats1D<T>::getIQR() const {
  if (iqr) return *iqr.get();
  if (!sorted) {
    sort(array, array + size);
    sorted = !sorted;
  }
  Stats1D<T>::getSummary();
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
struct Stats1D<T>::Summary Stats1D<T>::getSummary() const {
  if (summary) return *summary.get();
  if (!sorted) {
    sort(array, array + size);
    sorted = !sorted;
  }
  struct Summary fn;
  fn.min = array[0];
  fn.max = array[n - 1];

  double q1_ind = .25 * size;
  double mid_ind = .5 * size;
  double q3_ind = .75 * size;
  double epsilon = 1 / 10000;

  if (q1_ind % 1 > epsilon)
    fn.q1 = (array[floor(q1_ind)] + array[ceil(q1_ind)]) / 2;
  if (mid_ind % 1 > epsilon)
    fn.median = (array[floor(mid_ind)] + array[ceil(mid_ind)]) / 2;
  if (q3_ind % 1 > epsilon)
    fn.q3 = (array[floor(q3_ind)] + array[ceil(q3_ind)]) / 2;

  fivenum = make_unique<struct Summary>(fn);
  return *summary.get();
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
double Stats1D<T>::getStdDev() const {
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
double Stats1D<T>::getVariance() const {
  if (variance) return *variance.get();
  double mean_tmp = getMean();
  double sum = 0;
  for (const auto& i : array) {
    sum += pow(i - mean_tmp, 2);
  }
  variance = make_unique<double>(sum / (size - 1));
  return *variance.get();
}