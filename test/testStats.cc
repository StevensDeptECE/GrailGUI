#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "unistd.h"
#include "xp/Stats.hh"

using namespace std;

int testInt[11];
vector<int64_t> test64(10);
float testFloat[10];
double testDouble[50];
vector<double> testBigDouble(50);

void initData() {
  string grailPath = getenv("GRAIL");
  grailPath += "/test/res/sample_nums.txt";
  ifstream file(grailPath);
  string str;

  for (int i = 0; i < 11; i++) {
    getline(file, str);
    testInt[i] = stoi(str);
  }
  for (int i = 0; i < 10; i++) {
    getline(file, str);
    test64[i] = stoll(str);
  }
  for (int i = 0; i < 10; i++) {
    getline(file, str);
    testFloat[i] = stof(str);
  }
  for (int i = 0; i < 50; i++) {
    getline(file, str);
    testDouble[i] = stod(str);
  }

  default_random_engine generator;
  uniform_int_distribution<int> distribution(INT32_MIN, INT32_MAX);

  for (int i = 0; i < 50; i++) {
    testBigDouble[i] = testDouble[i] * distribution(generator);
  }
}

int main() {
  initData();

  Stats1D<int> statsInt(testInt, 11);
  Stats1D<int64_t> stats64(&test64[0], 10);
  Stats1D<float> statsFloat(testFloat, 10);
  Stats1D<double> statsDouble(testDouble, 50);
  Stats1D<double> statsBigDouble(&testBigDouble[0], 50);

  cout << statsInt << "\n\n"
       << stats64 << "\n\n"
       << statsFloat << "\n\n"
       << statsDouble << "\n\n"
       << statsBigDouble;
}