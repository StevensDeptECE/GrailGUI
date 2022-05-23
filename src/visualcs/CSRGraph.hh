#pragma once
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>  // we don't have a queue, so use STL

#include "util/Ex.hh"
#include "util/Stack.hh"
#include "util/wtime.h"
#include "visualcs/Graph.hh"

template <typename VertexIndex_t, typename EdgeIndex_t, typename Weight_t>
class CSRGraph : public Graph {
 private:
  static inline off_t fsize(const char* filename) {
    struct stat st;
    if (stat(filename, &st) == 0) return st.st_size;
    return -1;
  }

  /*
    not using this yet. We can't figure out a convenient way to share the code
    between the three vectors, so it's cut and past in the load file for now
   */
  template <typename T>
  void loadCSRFile(const char filename[], uint64_t fileLen) {
    FILE* file = fopen(filename, "rb");
    if (file == nullptr) throw Ex(__FILE__, __LINE__, "Can't open");
    T* csrVec = nullptr;

    if (posix_memalign((void**)&csrVec, getpagesize(),
                       sizeof(T) * (fileLen + 1)))
      throw Ex(__FILE__, __LINE__, "posix_memalign");

    size_t ret = fread(csrVec, sizeof(T), fileLen, file);
    assert(ret == fileLen);
    fclose(file);
#if 0
    adjLen = csrVec[vert_count];
    std::cout << "Expected edge count: " << csrVec[vert_count] << "\n";
    assert(tmp_beg_pos[vert_count]>0);
#endif
  }

 public:
  VertexIndex_t startIndexLen;  // this is actually the number of vertices (V)
                                // the actual length = +1
  EdgeIndex_t adjLen;           // the number of edges in the graph
  EdgeIndex_t weightLen;  // this should be the same as the number of edges
  EdgeIndex_t* startIndex;
  VertexIndex_t* adjacency;
  Weight_t* weight;
  CSRGraph()
      : Graph(),
        startIndexLen(0),
        adjLen(0),
        weightLen(0),
        startIndex(nullptr),
        adjacency(nullptr),
        weight(nullptr) {}
  ~CSRGraph() {
    // TODO: add matching deallocate, is this free or something else?
  }
  CSRGraph(const CSRGraph& orig) = delete;
  CSRGraph& operator=(const CSRGraph& orig) = delete;

  uint32_t getV() const { return startIndexLen; }
  uint64_t getE() const { return adjLen; }

  Weight_t getW(VertexIndex_t src, VertexIndex_t dest) const {
    return 0;  // TODO: make this work
  }
  bool isAdjacent(VertexIndex_t src, VertexIndex_t dest) const {
    for (EdgeIndex_t i = startIndex[src]; i < startIndex[src + 1]; i++)
      if (adjacency[i] == dest) return true;
    return false;
  }

  void getEdge(EdgeIndex_t i, VertexIndex_t* v1, VertexIndex_t* v2) {}
  VertexIndex_t countAdjacencies(VertexIndex_t v) {
    return startIndex[v + 1] - startIndex[v];
  }

  CSRGraph(const char csrBeginName[], const char csrAdjName[],
           const char weightName[]) {
    double tm = wtime();
    FILE* file = nullptr;
    size_t ret;

    startIndexLen = fsize(csrBeginName) / sizeof(EdgeIndex_t) -
                    1;  // one extra index for last one
    adjLen = fsize(csrAdjName) / sizeof(VertexIndex_t);

    // Read CSR starting Index
    file = fopen(csrBeginName, "rb");
    if (file == nullptr) throw "fopen fail csrbegin";
    //      throw Ex(__FILE__, __LINE__, "Can't open");
    startIndex = nullptr;

    // if(posix_memalign((void **)&startIndex, getpagesize(),
    // sizeof(EdgeIndex_t)*(startIndexLen+1)))
    startIndex = new EdgeIndex_t[startIndexLen + 1];
    // throw "posix memalign";
    //       throw Ex2(__FILE__, __LINE__, "posix_memalign");

    ret = fread(startIndex, sizeof(EdgeIndex_t), startIndexLen + 1, file);
    assert(ret == startIndexLen + 1);
    fclose(file);

    assert(adjLen == startIndex[startIndexLen]);
    std::cout << "Expected edge count: " << adjLen << "\n";

    // Read adjacency list
    file = fopen(csrAdjName, "rb");
    if (file == nullptr) throw "fopen fail adj";
    //      throw Ex(__FILE__, __LINE__, "Can't open");
    adjacency = nullptr;

    // if(posix_memalign((void **)&adjacency, getpagesize(),
    // sizeof(VertexIndex_t)*(adjLen)))
    adjacency = new VertexIndex_t[adjLen];
    // throw "fopen fail memalign";
    //       throw Ex(__FILE__, __LINE__, "posix_memalign");

    ret = fread(adjacency, sizeof(VertexIndex_t), adjLen, file);
    assert(ret == adjLen);
    fclose(file);
    V = startIndexLen;
    E = adjLen;

    std::cout << "Graph load (success): " << startIndexLen << " verts, "
              << adjLen << " edges " << wtime() - tm << " second(s)\n";
  }

  typedef void (*FuncVert)(uint32_t src, uint32_t dest);
  /*

   */
  void dfs(VertexIndex_t v, FuncVert f) {
    bool visited[getV()] = {false};
    Stack<VertexIndex_t> toVisit(getV());
    toVisit.push(v);
    visited[v] = true;
    f(v, v);
    while (!toVisit.isEmpty()) {
      VertexIndex_t src = toVisit.pop();
      for (EdgeIndex_t i = startIndex[src]; i < startIndex[src + 1]; i++) {
        VertexIndex_t dest = adjacency[i];
        if (!visited[dest]) {
          toVisit.push(dest);
          visited[dest] = true;
          // update the graph
          f(src, dest);
        }
      }
    }
  }

  /*

   */
  void bfs(uint32_t v, FuncVert f) {
    bool visited[getV()] = {false};
    std::queue<VertexIndex_t> toVisit;
    toVisit.enqueue(v);
    visited[v] = true;
    f(v, v);
    while (!toVisit.empty()) {
      VertexIndex_t src = toVisit.dequeue();
      for (EdgeIndex_t i = startIndex[src]; i < startIndex[src + 1]; i++) {
        VertexIndex_t dest = adjacency[i];
        if (!visited[dest]) {
          toVisit.enqueue(dest);
          visited[dest] = true;
          // update the graph
          f(src, dest);
        }
      }
    }
  }
  uint32_t* computeHistogram() const {
    uint32_t* hist = new uint32_t[getV()] = {0};
    for (VertexIndex_t v = 0; v < getV(); v++) hist[countAdjacencies(v)]++;
    return hist;
  }

  void printHistogram() const {
    uint32_t hist = computeHistogram();
    for (VertexIndex_t i = 0; i < getV(); i++)
      if (hist[i] != 0) {
        std::cout << i << ": " << hist[i] << '\n';
      }
    delete[] hist;
  }

  struct VertexCount {
    VertexIndex_t v;
    VertexIndex_t countAdj;
    VertexCount() : v(0), countAdj(0) {}
    VertexCount(VertexIndex_t v, VertexIndex_t countAdj)
        : v(v), countAdj(countAdj) {}
  };
  static bool sortDescCount(const VertexCount& a, const VertexCount& b) {
    return a.countAdj > b.countAdj;
  }
  const VertexIndex_t* group() {
    VertexIndex_t V = getV();
    VertexCount* v = new VertexCount[V];

    for (VertexIndex_t i = 0; i < V; i++)
      v[i] = VertexCount(i, countAdjacencies(i));

    std::sort(v, v + V, sortDescCount);
    VertexIndex_t* vertices = new VertexIndex_t[V];
    for (VertexIndex_t i = 0; i < V; i++) vertices[i] = v[i].v;
    delete[] v;

    return vertices;
  }

  void cluster() {
    constexpr int n = 4;
    uint32_t V = getV();
    uint32_t avg = V / (n * n);
    std::vector<VertexIndex_t> grid[n][n];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) grid[i][j].reserve(avg);
    for (int i = 0; i < V; i++) {
    }
  }
};
