#include "util/Config.hh"

#include <iostream>

void Config::save(const string& filename) {
  cerr << "Saving Config to " << filename << '\n';
  ofstream saveFile(filename);
  // for (auto p = f.begin(); p != f.end(); ++p) {
  // 	cout << p->first << "==>" << p->second << '\n';
  // }
}

const regex Config::commentOrWhitespace("#.*$|^\\s*$");

// this reads vector in form of [double,double,double]
const regex Config::checkVector(R"(\[\d+(.\d*),\d+(.\d*),\d+(.\d*)\])");

void Config::load(const string& name) {
  // Should this function return a map instead?
  // TODO: Change the hashmap allocator at the end of this function
  // Function to read the config file and update it to the hashmap for the
  // configuration
  string line, key, val;
  int flag;
  ifstream reader(name, ios::in);

  while (getline(reader, line), !reader.eof()) {
    cerr << "LINE: " << line << '\n';
    // removing the comments and blank lines
    line = regex_replace(line, commentOrWhitespace, "");
    // If the line is empty, continue
    if (line == "") continue;
    int sep = line.find('\t');  // TODO: use regex to parse
    string name = line.substr(0, sep);
    string val = line.substr(sep + 1);
    if (types.find(name) != types.end()) {
      Type t = types[name];

      f.insert(pair<string, Sym>(name, Sym(t, val)));
      // Sym s(t, val);
      // f[name] = s;
      // f[name] = s;
      // f[name] = new Sym(t, val);
      // f[name] = Sym(t, const &val);
      cout << int(t) << ": " << val << '\n';
    }
  }
  reader.close();
}

/*
        Define fields that are mandatory in the config file,
        and what types they must be
*/
void Config::mandatory(int count...) {
  va_list args;
  va_start(args, count);
  for (int i = 0; i < count; i++) {
    string str = va_arg(args, const char*);
    Type m = (Type)va_arg(args, int);
    types[str] = m;
  }
  va_end(args);
}

void Config::optional(int count...) {
  va_list args;
  va_start(args, count);
  for (int i = 0; i < count; i++) {
    string str = va_arg(args, const char*);
    Type m = (Type)va_arg(args, int);
    optTypes[str] = m;
  }
  va_end(args);
}
