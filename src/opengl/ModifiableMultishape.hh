#include "opengl/Image.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "util/DynArray.hh"

class ModifiableMultiShape : public StyledMultiShape2D {
 private:
  struct SolidInfo {
    uint32_t startVertexIndex;  // starting position within vertices
    uint32_t size;             // how many floating point numbers does this use?
    uint32_t startSolidIndex;  // start position in sbo
    uint32_t numInVertices;

    SolidInfo(uint32_t startVertexIndex, uint32_t size,
              uint32_t startSolidIndex, uint32_t numInVertices)
        : startVertexIndex(startVertexIndex),
          size(size),
          startSolidIndex(startSolidIndex),
          numInVertices(numInVertices){};
  };
  struct LineInfo {
    uint32_t startVertex;     // starting position within vertices
    uint32_t size;            // how many floating point numbers does this use?
    uint32_t startLineIndex;  // start position in lbo
    uint32_t numInVertices;

    LineInfo(uint32_t startVertex, uint32_t size, uint32_t startLineIndex,
             uint32_t numInVertices)
        : startVertex(startVertex),
          size(size),
          startLineIndex(startLineIndex),
          numInVertices(numInVertices){};
  };
  DynArray<SolidInfo> solidList;
  DynArray<LineInfo> lineList;
  Tab* tab;

 public:
  // create a rectangle, returning a handle to delete it if needed?
  float x = 10;
  float y = 10;
  float w = 150;
  float h = 150;
  const glm::vec4& c = grail::red;

  ModifiableMultiShape(Canvas* c, const Style* s, Tab* tab,
                       uint32_t solidListCount = 1024,
                       uint32_t lineListCount = 1024)
      : StyledMultiShape2D(c, s), solidList(1024), lineList(1024), tab(tab){};

  uint32_t addfillRectangle() {
    uint32_t index = solidList.size();
    uint32_t startVertexIndex = vertices.size();
    uint32_t startSolidIndex = solidIndices.size();
    uint32_t numInVertices = elemPerVert * 4;
    uint32_t points = 4;
    solidList.add(
        SolidInfo(startVertexIndex, points, startSolidIndex, numInVertices));
    // change StyledMultiShape2d so if you are bigger than the original size,
    // reallocate
    StyledMultiShape2D::fillRectangle(x, y, w, h, c);
    init();
    tab->getParentWin()->setUpdate();
    x += 200;
    return index;  // solidList.size() - 1;
  }

  uint32_t drawRectangle() {
    uint32_t index = lineList.size();
    uint32_t startVertexIndex = vertices.size();
    uint32_t startLineIndex = lineIndices.size();
    uint32_t numInVertices = elemPerVert * 4;
    uint32_t points = 4;
    lineList.add(
        LineInfo(startVertexIndex, points, startLineIndex, numInVertices));
    // change StyledMultiShape2d so if you are bigger than the original size,
    // reallocate
    StyledMultiShape2D::drawRectangle(x, y, w, h, c);
    init();
    tab->getParentWin()->setUpdate();
    x += 200;
    return index;  // solidList.size() - 1;
  }

  // TODO: come up with easy API to modify existing StyledMultiShape2Ds...
  void removeSolid(uint32_t index) {
    x -= 200;
    std::vector<float>::iterator iterator =
        vertices.begin() + solidList[index].startVertexIndex;
    for (uint32_t i = 0; i < solidList[index].numInVertices; i++) {
      vertices.erase(iterator);
    }
    std::vector<uint32_t>::iterator iterator2 =
        solidIndices.begin() + solidList[index].startSolidIndex;
    for (int i = 0; i < 6; i++) {
      solidIndices.erase(iterator2);
    }
    init();
    tab->getParentWin()->setUpdate();
  }

  void removeline(uint32_t index) {
    x -= 200;
    std::vector<float>::iterator iterator =
        vertices.begin() + lineList[index].startLineIndex;
    for (uint32_t i = 0; i < lineList[index].numInVertices; i++) {
      vertices.erase(iterator);
    }
    std::vector<uint32_t>::iterator iterator2 =
        lineIndices.begin() + lineList[index].startLineIndex;
    for (int i = 0; i < 6; i++) {
      lineIndices.erase(iterator2);
    }
    init();
    tab->getParentWin()->setUpdate();
  }
  void removeLastSolid() {}
  void removeLastLine() {}
};