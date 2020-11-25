#pragma once
#include <string>
#include <vector>

class CSVParser {
private:
  enum class CSVState {
    UnquotedField,
    QuotedField,
    QuotedQuote
  };

  static std::vector<std::string> readCSVRow(const std::string &row);

public:
  /// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
  static std::vector<std::vector<std::string>> readCSV(const char filename[]);
};
