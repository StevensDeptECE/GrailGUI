#pragma once
#include <limits>
#include <iostream>


class MatrixGraph {
private:
  uint32_t V;
  double* e;
  uint32_t edgeCount;

public:
  constexpr static double INF = std::numeric_limits<double>::infinity();
  MatrixGraph(int V) : V(V), e(new double[V*V]) {
    for (int i = 0; i < V*V; i++)
      e[i] = INF;
    edgeCount = 0;
  }

  ~MatrixGraph() {
    delete [] e;
  }
  MatrixGraph(const MatrixGraph& orig) = delete;
  MatrixGraph& operator =(const MatrixGraph& orig) = delete;

  uint32_t getV() const { return V; }
  uint32_t getE() const { return edgeCount; }
  double getW(uint32_t from, uint32_t to) const { return e[from * V + to]; }
  void setW(uint32_t from, uint32_t to, double v) {e[from * V + to] = v; }

  double operator ()(int from, int to) const {  return e[from * V + to]; }
  double& operator ()(int from, int to) {  return e[from * V + to]; }

  friend std::istream& operator >>(std::istream& s, MatrixGraph& g) {
    delete [] g.e;
    s >> g.V;
    g.e = new double[g.V*g.V];
    double edge;
    for (int i = 0; i < g.V*g.V; i++) {
      s >> edge;
      if (edge > 1e50)
        edge = INF;
      else
        g.edgeCount++;
      g.e[i] = edge;
    }
    return s;
  }
  void dfs(int v);
  
};
