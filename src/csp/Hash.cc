#include "csp/Hash.hh"

using namespace std;

void HashMap::grow() {
  vector<Node*> temp(size_ * 4);
  size_t k = 0;
  Node* t;
  for (size_t i = 0; i < size_; i++) {
    while (v_[i] != nullptr) {
      k = hash(const_cast<char*>(v_[i]->str.c_str())) % (size_ * 4);
      temp[k] = new Node(v_[i]->str, v_[i]->servlet, temp[k]);
      t = v_[i];
      v_[i] = v_[i]->next;
      delete t;
    }
  }
  size_ *= 4;
  v_ = temp;
}

HashMap::HashMap(size_t size, size_t used)
    : size_(size), v_(size), used_(used) {}

HashMap::~HashMap() {
  for (auto i : v_) {
    Node* t;
    for (Node* p = i; p != nullptr; p = t) {
      t = p->next;
      delete p;
    }
  }
}

size_t HashMap::hash(const char* str) const {
  size_t h = 1315423911;
  while (!isspace(*str)) {
    h ^= ((h << 5) + (*str++) + (h >> 2));
  }
  return (h & 0x7FFFFFFF);
}

void HashMap::add(const char* url, HttpServlet* servlet) {
  if (2 * used_ > size_) {
    grow();
  }
  size_t h = hash(url);
  int k = h % size_;
  Node* p = v_[k];
  while (p != nullptr) {
    if (p->str == url) {
      p->break;
    }
    p = p->next;
  }
  if (!p) {
    v_[k] = new Node(url, servlet, v_[k]);
    used_++;
  }
}

HttpServlet* HashMap::get(const char* url) const {
  size_t k = hash(url) % size_;
  Node* p = v_[k];
  while (p) {
    if (url == p->str) {
      return p->servlet;
    }
    p = p->next;
  }
  return nullptr;
}
