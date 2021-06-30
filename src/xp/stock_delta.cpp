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
  int16_t open_delta;  // Convert to signed int, instead of using neg bit
  uint16_t high_delta;
  uint16_t low_delta;
  uint16_t close_delta;
  uint16_t volume_approximate;
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
        close_delta(0),
        volume_approximate(0),
        empty_stock(true),
        first_day(false) {}

  // Remove negative bit
  friend ostream& operator<<(ostream& o, const Stock& t) {
    if (t.first_day) {
      return o << setprecision(5) << t.open_price << " " << t.high_delta << " "
               << t.low_delta << " " << t.close_delta << " "
               << t.volume_approximate << '\n';
    } else {
      return o << t.open_delta << " " << t.high_delta << " " << t.low_delta
               << " " << t.close_delta << " " << t.volume_approximate << '\n';
    }
  }

  // Make sure first_day is false, set true in delta()
  friend istream& operator>>(istream& i, Stock& t) {
    char symbol[32];
    char buf[1024];
    t.empty_stock = false;
    t.first_day = false;
    i.getline(buf, sizeof(buf));
    uint32_t mm, dd, yyyy;
    double decOpen, decHi, decLow, decClose;
    uint32_t openInterest;
    char sep;  // store the bogus - and , separators
    sscanf(buf, "%u-%u-%u,%f,%f,%f,%f,%lu,%u", &yyyy, &mm, &dd, &t.open_price,
           &t.high, &t.low, &t.close, &t.volume, &openInterest);
    return i;
  }

  // Change default to nullptr instead of empty stock,
  void delta(Stock* prev_day = nullptr) {
    if (prev_day == nullptr) {
      first_day = true;
    }
    // Since prev_day defaults to null, check if it exists
    double prev_day_close =
        (!prev_day) ? open_price : prev_day->get_close_price();
    double open_delta_double = open_price - prev_day_close;
    double high_delta_double = high - open_price;
    double low_delta_double = open_price - low;
    double close_delta_double = close - low;
    int decimal = 5;
    open_delta = open_delta_double * pow(10, decimal);
    high_delta = high_delta_double * pow(10, decimal);
    low_delta = low_delta_double * pow(10, decimal);
    close_delta = close_delta_double * pow(10, decimal);
    volume_approximate = volume / 10000;
  }

  float get_close_price() { return close; }
  bool isEmpty() { return empty_stock; }

  // Writes stock out to ostream as bits to be read in later
  friend void binwrite(ofstream& o, const Stock& s) {
    if (s.first_day)  // write base price if first_day, else write delta
      o.write((char*)&s.open_price, sizeof(float));
    else
      o.write((char*)&s.open_delta, sizeof(int16_t));

    o.write((char*)&s.high_delta, sizeof(uint16_t));
    o.write((char*)&s.low_delta, sizeof(uint16_t));
    o.write((char*)&s.close_delta, sizeof(uint16_t));
    o.write((char*)&s.volume_approximate, sizeof(uint16_t));
  }
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
      if (stocks.size() > 0)
        s.delta(&stocks[stocks.size() - 1]);
      else
        s.delta();
      stocks.push_back(s);
    }
  };
  friend ostream& operator<<(ostream& o, const Compress_stock& c) {
    for (int i = 0; i < c.stocks.size(); i++) {
      o << c.stocks[i];  // Fix space at beginning of lines
    }
    return o << '\n';
  }

  // Creates output stream and writes each Stock
  friend void binwrite(const char* filename, const Compress_stock& c) {
    ofstream stream(filename, ios::binary);
    for (const auto& elem : c.stocks) {
      binwrite(stream, elem);
    }
  }
};

int main() {
  const Compress_stock c("aapl.txt");
  ofstream outfile("aapl_delta.txt");
  outfile << c;
  binwrite("aapl_delta.bin", c);
}
