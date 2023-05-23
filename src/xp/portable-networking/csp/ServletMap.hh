#pragma once

/**
         Mapping to look up URI and rapidly turn into pointer to the servlet to
   execute For speed, no memory is allocated during lookup.  The keys can be
   strings, but lookup is a pointer to char within a buffer and a length

         @author: Dov Kruger

*/

#include <string>
#include <vector>

#include "csp/HttpServlet.hh"

const std::string EMPTYSTRING("");

class ServletMap {
 private:
  struct Node {
    std::string name;
    HttpServlet* s;
    Node(const std::string& name, HttpServlet* s) : name(name), s(s) {}
  };
  std::vector<Node> table;
  size_t used;
  size_t hash(const char* s, size_t size) const {
    size_t hashVal = size * 31;
    for (size_t i = 0; i < size; i++)
      hashVal = (hashVal * 1023007001) + s[i] + (hashVal >> 17);
#if 0
		for ( ; size > sizeof(uint64_t); size -= sizeof(uint64_t))
			hashVal = hashVal * (1 + 1024 + (1 << 17)) + *(uint64_t*)s + hashVal >> 13;
#endif
    return hashVal % table.size();
  }

  bool compare(const std::string& name, const char* n, size_t len) const {
    if (name.size() != len) return false;
    for (uint32_t i = 0; i < len; ++i)
      if (name[i] != n[i]) return false;
    return true;
  }

 public:
  ServletMap(uint32_t initialSize = 32768) : used(0) {
    table.reserve(initialSize);
    for (uint32_t i = 0; i < initialSize; i++)
      table.push_back(Node("", nullptr));
  }

  void add(const std::string& name, HttpServlet* s) {
    size_t i = hash(name.c_str(), name.size());

    while (!compare(table[i].name, name.c_str(), name.size())) {
      if (table[i].name == "") {
        table[i].name.reserve(name.size());
        table[i].name = name;
        table[i].s = s;
        used++;
        return;
      }
      i = i + 1;
      if (i >= table.size()) i = 0;
    }
  }

  HttpServlet* get(const char* s, size_t size) const {
    size_t i = hash(s, size);
    while (table[i].name != EMPTYSTRING) {
      if (compare(table[i].name, s, size)) {
        return table[i].s;
      }
      i = i + 1;
      if (i >= table.size()) {
        i = 0;
      }
    }
    return nullptr;
  }
};
