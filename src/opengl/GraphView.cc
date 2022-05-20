#include "opengl/GraphView.hh"

#include <glm/glm.hpp>

#include "opengl/Colors.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;
void GraphView::addVert(uint64_t v, float x, float y, uint8_t color,
                        Point points[]) {
  mVertices.fillCircle(x, y, vertR, 1, defaultStyle);
  // if (v < 10)
  //   cout << "points: " << x << " " << y << endl;
  points[v] = Point(x, y);
}

void GraphView::addEdge(uint64_t i, float x1, float y1, float x2, float y2,
                        uint8_t initialEdgeColor) {
  mEdges.drawLine(x1, y1, x2, y2, defaultStyle);
}

void GraphView::updateVert(uint64_t v, uint8_t color) {
  // j is the position of the combined edge/verts made... should be changed
  // later e.g. addVert(), addVert()
  // ==> vert1 = pos 0
  // ==> vert2 = pos 1
  // ==> edge3 = pos 3
  mVertices.updateColors(v, colorMap[color].r, colorMap[color].g,
                         colorMap[color].b);
  //  c->init();
}

void GraphView::updateEdge(uint64_t i, uint8_t color) {
  mEdges.updateColors(i, colorMap[color].r, colorMap[color].g,
                      colorMap[color].b);
}

void GraphView::init() {
  mVertices.init();
  mEdges.init();
}

void GraphView::render() {
  mEdges.render();
  mVertices.render();
}

void GraphView::process_input(Inputs*, float dt) {}

void GraphView::update() {}

typedef uint64_t VertexIndex_t;
typedef uint64_t EdgeIndex_t;

void GraphView::addEdges(const Point vertexPos[], uint8_t initialEdgeColor) {
  uint64_t dest;
  uint64_t edgeNumber = 0;
  for (VertexIndex_t src = 0; src < g->startIndexLen; src++) {
    EdgeIndex_t myEdgeBegin = g->startIndex[src];
    EdgeIndex_t myEdgeNext = g->startIndex[src + 1];
    // cout << src << " adj: ";
    for (EdgeIndex_t i = myEdgeBegin; i < myEdgeNext; i++) {
      dest = g->adjacency[i];
      // cout << dest << ' ';
      // if (src < 20)
      float x1 = vertexPos[src].x, y1 = vertexPos[src].y,
            x2 = vertexPos[dest].x, y2 = vertexPos[dest].y;
#if 0
      if (verbose) {
        printf("vertexPos[%ld]  x= %f  y = %f  \t\t", src, x1, y1);
        printf("vertexPos[%ld]  x= %f  y = %f\n", dest, x2, y2);
      }
#endif
      // TODO: move inward by vertR to avoid drawing under the vertex
      // if (src < 11)
      //   printf("%d, %f, %f, %f, %f, %d\n", edgeNumber++, x1, y1, x2, y2,
      //   initialEdgeColor);
      addEdge(edgeNumber++, x1, y1, x2, y2, initialEdgeColor);
    }
    //    cout << '\n';
  }
}

void circleLayout(GraphView* gv, float xc, float yc, float r, uint64_t V,
                  uint8_t initialVertexColor, uint8_t initialEdgeColor) {
  double pathLen = 2 * PI<double> * r;
  double dt = 2 * PI<double> / V;
  float vertR =
      pathLen / V / 4.0;  // leave some blank space surrounding each vertex
  double t = 0;
  gv->setVertexRadius(vertR);
  Point* vertexPos = new Point[gv->getE()];
  for (uint64_t v = 0; v < V; v++, t += dt) {
    double x = xc + r * cos(t), y = yc + r * sin(t);
    gv->addVert(v, x, y, initialVertexColor, vertexPos);
  }
  gv->addEdges(vertexPos, initialEdgeColor);
  delete[] vertexPos;
}

void multiCircleLayout(GraphView* gv, float xc, float yc, float rinner,
                       float ringSpace, uint32_t numRings, uint64_t V,
                       uint8_t initialVertexColor, uint8_t initialEdgeColor) {
  double r = rinner;
  double totalDist = 0;
  for (int i = 0; i < numRings; i++, r += ringSpace) {
    double circumference = 2 * PI<double> * r;
    totalDist += circumference;
  }
  double spacePerVertex = min(totalDist / V, double(ringSpace * .66));
  r = rinner;
  gv->setVertexRadius(spacePerVertex / 3);
  spacePerVertex *= 2;

  Point* vertexPos = new Point[gv->getE()];
  const uint64_t* vertices = gv->getGraph()->group();

  uint64_t v = 0;
  for (int i = 0; i < numRings; i++, r += ringSpace) {
    double circumference = 2 * PI<double> * r;
    uint32_t vertInThisRing = uint32_t(circumference / spacePerVertex);
    double vertSpacingThisRing = 2 * PI<double> / vertInThisRing;
    double t = 0;
    for (int j = 0; j < vertInThisRing && v < V;
         j++, t += vertSpacingThisRing, v++) {
      double x = xc + r * cos(t), y = yc + r * sin(t);
      gv->addVert(vertices[v], x, y, initialVertexColor, vertexPos);
    }
  }
  gv->addEdges(vertexPos, initialEdgeColor);
  delete[] vertexPos;
}

void grid(GraphView* gv, float x0, float y0, float w, float h, uint32_t numRows,
          uint32_t numCols, uint64_t V, uint8_t initialVertexColor,
          uint8_t initialEdgeColor) {
  double spacePerVertex = std::min(w / numCols, h / numRows);
  gv->setVertexRadius(spacePerVertex / 2);
  double x, y;
  double dx = w / numCols, dy = h / numRows;
  Point* vertexPos = new Point[gv->getE()];
  y = y0;
  for (uint64_t v = 0, i = 0; i < numRows; i++, y += dy) {
    x = x0;
    for (uint64_t j = 0; j < numCols; j++, v++, x += dx) {
      gv->addVert(v, x, y, initialVertexColor, vertexPos);
    }
  }
  gv->addEdges(vertexPos, initialEdgeColor);
  delete[] vertexPos;
}

void bipartite(GraphView& gv, float x1, float x2) {}
