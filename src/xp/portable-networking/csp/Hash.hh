#pragma once

#include <string>

class Hash {
 private:
  struct Node {
    std::string name;
    HTTPServlet* s;
    Node(const char* n, HTTPServlet* s) : name(n), s(s) {}
  };
  vector<list<Node*> > table;
  Hash() {}
  ~Hash();
  Hash(const Hash& orig) = delete;
  Hash& operator=(const Hash& orig) = delete;
  size_t hash(const char* str) const;

  void add(const char* str);
  const char* get(const char* str) const;
};
