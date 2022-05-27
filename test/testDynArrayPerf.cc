#include <functional>
#include <memory>
#include <vector>

#include "util/Benchmark.hh"
#include "util/DynArray.hh"

using namespace std;

class PtrObject {
 private:
  vector<int> data;
  string name;

 public:
  PtrObject(int initialSize, string name) : data(initialSize), name(name) {}

  int& get(int index) { return data[index]; }
  void add(int index, int value) { data.insert(data.begin() + index, value); }
};

class Vec3d {
 private:
  double x, y, z;

 public:
  Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
  void set(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};

int main() {
  auto add_raw_ptr_object = [] {
    DynArray<PtrObject*> a(100);
    for (int i = 0; i < 100; i++) {
      a.add(new PtrObject(50, "fred"));
      for (int j = 0; j < 50; j++) {
        a[i]->add(j, i);
      }
      for (int j = 0; j < 50; j++) {
        for (int k = 0; k < 1000; k++) {
          a[i]->get(j)++;
        }
      }
    }
    for (int i = 0; i < 100; i++) {
      delete a[i];
    }
  };

  auto emplace_raw_ptr_object = [] {
    DynArray<PtrObject*> a(100);
    for (int i = 0; i < 100; i++) {
      a.emplace_back(new PtrObject(50, "fred"));
      for (int j = 0; j < 50; j++) {
        a[i]->add(j, i);
      }

      for (int j = 0; j < 50; j++) {
        for (int k = 0; k < 1000; k++) {
          a[i]->get(j)++;
        }
      }
    }
    for (int i = 0; i < 100; i++) {
      delete a[i];
    }
  };

  /*  auto add_unique_ptr_object = [] {
      DynArray<unique_ptr<PtrObject>> a(10);
      for (int i = 0; i < 10; i++) {
        a.add(make_unique<PtrObject>(50, "fred"));
        for (int j = 0; j < 50; j++) {
          a[j]->add(j, i);
        }
      }
    };
  */
  auto emplace_unique_ptr_object = [] {
    DynArray<unique_ptr<PtrObject>> a(100);
    for (int i = 0; i < 100; i++) {
      a.emplace_back(make_unique<PtrObject>(50, "fred"));
      for (int j = 0; j < 50; j++) {
        a[i]->add(j, i);
      }
      for (int j = 0; j < 50; j++) {
        for (int k = 0; k < 1000; k++) {
          a[i]->get(j)++;
        }
      }
    }
  };

  CBenchmark::benchmark("PtrObject raw ptr add", 1e3, bind(add_raw_ptr_object));
  CBenchmark::benchmark("PtrObject raw ptr emplace", 1e3,
                        bind(emplace_raw_ptr_object));
  // CBenchmark::benchmark("PtrObject unique add", 1e6,
  // bind(add_unique_ptr_object));
  CBenchmark::benchmark("PtrObject unique emplace", 1e3,
                        bind(emplace_unique_ptr_object));
}