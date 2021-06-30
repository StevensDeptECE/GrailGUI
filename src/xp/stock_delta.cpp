#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Stock {
 private:
  float open_price;
  float high;
  float low;
  float close;
  uint64_t volume;
  uint16_t open_delta;
  uint16_t high_delta;
  uint16_t low_delta;
  uint16_t close_delta;
  uint16_t volume_approximate;
  char neg_bit : 6;
  bool empty_stock : 1;
  bool first_day : 1;

 public:
  Stock()
      : open_price(0),
        high(0),
        low(0),
        close(0),
        volume(0),
        open_delta(0),
        high_delta(0),
        low_delta(0),
        close_delta('0'),
        volume_approximate(0),
        empty_stock(1),
        first_day(0) {}
  friend ostream& operator<<(ostream& o, const Stock& t) {
    if (t.first_day) {
      return o << t.neg_bit << " " << setprecision(5) << t.open_price << " "
               << t.high_delta << " " << t.low_delta << " " << t.close_delta
               << " " << t.volume_approximate << '\n';
    } else {
      return o << t.neg_bit << " " << t.open_delta << " " << t.high_delta << " "
               << t.low_delta << " " << t.close_delta << " "
               << t.volume_approximate << '\n';
    }
  }
  friend istream& operator>>(istream& i, Stock& t) {
    char symbol[32];
    char buf[1024];
    t.empty_stock = false;
    i.getline(buf, sizeof(buf));
    uint32_t mm, dd, yyyy;
    double decOpen, decHi, decLow, decClose;
    uint32_t openInterest;
    char sep;  // store the bogus - and , separators
    sscanf(buf, "%u-%u-%u,%f,%f,%f,%f,%lu,%u", &yyyy, &mm, &dd, &t.open_price,
           &t.high, &t.low, &t.close, &t.volume, &openInterest);
    return i;
  }
  void delta(Stock prev_day = Stock()) {
    if (prev_day.isEmpty()) {
      first_day = true;
    }
    double open_delta_double = open_price - prev_day.get_close_price();
    double high_delta_double = high - open_price;
    double low_delta_double = open_price - low;
    double close_delta_double = close - low;
    int decimal = 5;
    neg_bit = (open_delta_double < 0) + 48;
    open_delta = open_delta_double * pow(10, decimal);
    high_delta = high_delta_double * pow(10, decimal);
    low_delta = low_delta_double * pow(10, decimal);
    close_delta = close_delta_double * pow(10, decimal);
    volume_approximate = volume / 10000;
  }

  float get_close_price() { return close; }
  bool isEmpty() { return empty_stock; }
};

class Compress_stock {
 private:
  vector<Stock> stocks;
  ifstream f;

 public:
  Compress_stock() {}
  Compress_stock(string name) : f(name.c_str()), stocks() {
    char buf[1024];
    f.getline(buf, sizeof(buf));
    Stock s;

    while (f >> s) {
      if (stocks.size() > 1)
        s.delta(stocks[stocks.size() - 1]);
      else
        s.delta();
      stocks.push_back(s);
    }
  };
  friend ostream& operator<<(ostream& o, const Compress_stock& c) {
    for (int i = 0; i < c.stocks.size(); i++) {
      o << c.stocks[i] << ' ';
    }
    return o << '\n';
  }
};

int main() {
  const Compress_stock c("aapl.txt");
  ofstream outfile("aapl_delta.txt");
  outfile << c;
}
