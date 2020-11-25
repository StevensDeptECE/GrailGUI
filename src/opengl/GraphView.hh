#pragma once

#include "opengl/StyledMultiShape2D.hh"
#include <glm/glm.hpp>
#include "opengl/GLMath.hh"
#include "visualcs/MatrixGraph.hh"
#include "visualcs/CSRGraph.hh"
#include "opengl/Canvas.hh"
#include <algorithm>
#include <iostream>

class Point {
public:
  float x,y;
  Point() {}
  Point(float x, float y) : x(x), y(y) {}
};

class GraphView : public Shape {
private:
  uint32_t pointsPerCircle;
  std::vector<float> vals; // values which are used to color the graph
  std::vector<glm::vec4> colorMap;
  CSRGraph<uint64_t, uint64_t, float>* g;
  StyledMultiShape2D mVertices;
  StyledMultiShape2D mEdges;
  float vertR;
	glm::vec4 defaultStyle;
  Canvas *c;
  const uint64_t* vertices;
public:
  CSRGraph<uint64_t, uint64_t, float>* getGraph() { return g; }
  void setVertexRadius(float vertR) {
    this->vertR = vertR;
  }
  void addColor(float r, float g, float b) { colorMap.push_back(glm::vec4(r, g, b, 0)); }
  void addVert(uint64_t v, float x, float y, uint8_t initialVertColor, Point points[]); // each vertex starts with color value 0
  void addEdge(uint64_t i, float x1, float y1, float x2, float y2, uint8_t initialEdgeColor);
  void updateVert(uint64_t v, uint8_t colorVal); // change the color of each point of vert with colorMap[colorVal]
  void updateEdge(uint64_t i, uint8_t colorVal); // change the color of the line colorMap[colorVal]
  GraphView(CSRGraph<uint64_t, uint64_t, float>& g, Canvas *c, Style* style, glm::vec4 s) : mVertices(style), mEdges(style), g(&g), defaultStyle(s), c(c) {
    c->addLayer(&mEdges);
    c->addLayer(&mVertices);
    vertices = g.group();
  }
  uint64_t getE() const { return g->adjLen; }
  // override Shape methods to draw us
  void process_input(Inputs*, float dt) override;
  void init() override;
  void render() override;
  void update() override;
  
  void addEdges(const Point vertexPos[], uint8_t initialEdgeColor);
};

void circleLayout(GraphView* gv, float xc, float yc, float r,
    uint64_t V, uint8_t initialVertexColor, uint8_t initialEdgeColor);
void multiCircleLayout(GraphView* gv, float xc, float yc, float rinner, float ringSpace,
                 uint32_t numRings, uint64_t V, uint8_t initialVertexColor, uint8_t initialEdgeColor);
void gridLayout(GraphView* gv, float x0, float y0, float w, float h,
          uint32_t numRows, uint32_t numCols, uint64_t V,
          uint8_t initialVertexColor, uint8_t initialEdgeColor);
void bipartiteLayout(GraphView& gv, float x1, float x2);
