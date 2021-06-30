#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Stock {
 private:
  double open_price;
  double high;
  double low;
  double close;
  int volume;
  uint32_t high_delta;
  uint32_t low_delta;
  uint32_t close_delta;
  uint32_t volume_approximate;

 public:
  Stock() {}
  friend ostream& operator<<(ostream& o, const Stock& t) {
    return o << t.open_price << " " << t.high_delta << " " << t.low_delta << " "
             << t.close_delta << " " << t.volume_approximate;
  }
  friend istream& operator>>(istream& i, Stock& t) {
    i.ignore(',');
    i >> t.open_price >> t.high >> t.low >> t.close >> t.volume;
    i.ignore('\n');
    return i;
  }
  void delta() {
    double high_delta_double = high - open_price;
    double low_delta_double = open_price - low;
    double close_delta_double = open_price - close;
    int decimal = 0;
    // Determine how many decimal places
    for (int i = 0; i < 6; i++) {
      high_delta_double *= 10;
      low_delta_double *= 10;
      close_delta_double *= 10;
      if (high_delta_double - (uint32_t)high_delta_double == 0 &&
          low_delta_double - (uint32_t)low_delta_double == 0 &&
          close_delta_double - (uint32_t)close_delta_double == 0) {
        decimal = i + 1;
        break;
      }
    }
    high_delta = high_delta_double * pow(10, decimal);
    low_delta = low_delta_double * pow(10, decimal);
    close_delta = close_delta_double * pow(10, decimal);
    volume_approximate = volume / 10000;
  }
};

class Compress_stock : public Stock {
 private:
  vector<Stock> Stocks;
  ifstream f;

 public:
  Compress_stock() {}
  Compress_stock(string name) : f(name.c_str()), Stocks() {
    char buf[1024];
    f.getline(buf, sizeof(buf));
    Stock s;

    while (f >> s) {
      s.delta();
      Stocks.push_back(s);
    }
  };
  friend ostream& operator<<(ostream& o, const Compress_stock c) {
    return o << c.Stocks << '\n';
  }
};

int main() {
  const Compress_stock c("aapl.txt");
  ofstream outfile;
  outfile.open("aapl_delta.txt");
  outfile << &c;
}
