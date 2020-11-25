#include <fstream>
#include <string>
#include <vector>
#include "CSVParser.hh"
using namespace std;

vector<string> CSVParser::readCSVRow(const string &row) {
  CSVState state = CSVState::UnquotedField;
  vector<string> fields {""};
  size_t i = 0; // index of the current field
  for (char c : row) {
    switch (state) {
      case CSVState::UnquotedField:
        switch (c) {
          case ',': // end of field
            fields.push_back(""); i++;
            break;
          case '"': state = CSVState::QuotedField;
                    break;
          default:  fields[i].push_back(c);
                    break; }
        break;
      case CSVState::QuotedField:
        switch (c) {
          case '"': state = CSVState::QuotedQuote;
                    break;
          default:  fields[i].push_back(c);
                    break; }
        break;
      case CSVState::QuotedQuote:
        switch (c) {
          case ',': // , after closing quote
            fields.push_back(""); i++;
            state = CSVState::UnquotedField;
            break;
          case '"': // "" -> "
            fields[i].push_back('"');
            state = CSVState::QuotedField;
            break;
          default:  // end of quote
            state = CSVState::UnquotedField;
            break; }
        break;
    }
  }
  return fields;
}

vector<vector<string>> CSVParser::readCSV(const char filename[]) {
  ifstream in;
  in.open(filename, ifstream::in);
  vector<vector<string>> table;
  string row;
  while (!in.eof()) {
    getline(in, row);
    if (in.bad() || in.fail()) {
      break;
    }
    auto fields = readCSVRow(row);
    table.push_back(fields);
  }
  return table;
}
