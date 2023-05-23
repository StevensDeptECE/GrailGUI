#pragma once

#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "csp/Insertion.hh"
#include "util/datatype.hh"

class Page {
 private:
  std::string staticContent;          // static content
  std::vector<Insertion> insertions;  // dynamic content
  // Make field for full size, update each time dynamic content is added (static
  // + dynamic)
  uint32_t size;
  uint32_t staticSize;
  void readOffsetVector(std::ifstream& infile);

 public:
  // Constructors
  Page() {}
  Page(const std::string& inputFile);
  Page(const std::string& staticContent, std::vector<Insertion> insertions)
      : staticContent(staticContent),
        insertions(insertions),
        size(staticContent.size()) {
    for (auto i : insertions) size += dynamicSize(i.d);
  }
  Page(const std::string& staticFile, const std::string& insertFile);
  // Getters
  char* getStaticPointer() { return &staticContent[0u]; }
  std::string getStaticContent() const { return staticContent; }
  std::vector<Insertion> getInsertions() const { return insertions; }
  uint32_t getSize() const { return size; }
  uint32_t getStaticSize() const { return staticSize; }

  // Methods
  void addDynamic(uint32_t offset, DataType d) {
    insertions.emplace_back(Insertion(offset, d));
    size += dynamicSize(d);
  }

  void addStatic(const std::string& t) { staticContent += t; }

  // TODO: make function to add static content
  bool isStatic() { return insertions.empty(); }
  uint32_t dynamicSize(DataType d);

  friend std::ostream& operator<<(std::ostream& s, const Page& p);
};

// TODO: Write function to write file