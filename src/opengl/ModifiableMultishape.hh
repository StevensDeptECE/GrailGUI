#include "opengl/Image.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "util/DynArray.hh"

class ModifiableMultiShape : public StyledMultiShape2D {
 private:
  struct SolidInfo {
    uint32_t startVertexIndex;  // starting position within vertices // how many
                                // floating point numbers does this use?
    uint32_t startSolidIndex;  // start position in sbo
    uint32_t numInVertices;
    uint32_t numInIndices;

    SolidInfo(uint32_t startVertexIndex, uint32_t numInIndices,
              uint32_t startSolidIndex, uint32_t numInVertices)
        : startVertexIndex(startVertexIndex),
          startSolidIndex(startSolidIndex),
          numInVertices(numInVertices),
          numInIndices(numInIndices){};
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

  ModifiableMultiShape(Canvas* c, const Style* s, Tab* tab,
                       uint32_t solidListCount = 1024,
                       uint32_t lineListCount = 1024)
      : StyledMultiShape2D(c, s), solidList(1024), lineList(1024), tab(tab){};

  uint32_t addfillRectangle(float x, float y, float w, float h,
                            const glm::vec4& c);
  // TODO: come up with easy API to modify existing StyledMultiShape2Ds...
  void removeSolid(uint32_t index);
};
