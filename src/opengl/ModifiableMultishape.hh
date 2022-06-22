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
    uint32_t ShapeIndices;

    SolidInfo(uint32_t startVertexIndex, uint32_t numInIndices,
              uint32_t startSolidIndex, uint32_t numInVertices,
              uint32_t ShapeIndices)
        : startVertexIndex(startVertexIndex),
          startSolidIndex(startSolidIndex),
          numInVertices(numInVertices),
          ShapeIndices(ShapeIndices),
          numInIndices(numInIndices){};
  };
  struct LineInfo {
    uint32_t lStartVertexIndex;  // starting position within vertices
    uint32_t startLineIndex;  // how many floating point numbers does this use?
    uint32_t lnumInVertices;  // start position in lbo
    uint32_t lnumInIndices;
    uint32_t lShapeIndices;

    LineInfo(uint32_t lStartVertexIndex, uint32_t lnumInIndices,
             uint32_t startLineIndex, uint32_t lnumInVertices,
             uint32_t lShapeIndices)
        : lStartVertexIndex(lStartVertexIndex),
          lnumInIndices(lnumInIndices),
          startLineIndex(startLineIndex),
          lShapeIndices(lShapeIndices),
          lnumInVertices(lnumInVertices){};
  };
  DynArray<SolidInfo> solidList;
  DynArray<LineInfo> lineList;
  uint32_t index;
  Tab* tab;

 public:
  // create a rectangle, returning a handle to delete it if needed?

  ModifiableMultiShape(Canvas* c, const Style* s, Tab* tab,
                       uint32_t solidListCount = 1024,
                       uint32_t lineListCount = 1024)
      : StyledMultiShape2D(c, s), solidList(1024), lineList(1024), tab(tab){};

  //====================== Util Functions =================//
  void UpdateSolidList(uint32_t startVertexIndex, uint32_t startSolidIndex);

  //=================== Solid Primatives ============//
  uint32_t addfillRectangle(float x, float y, float w, float h,
                            const glm::vec4& c);
  uint32_t addfillRoundRect(float x, float y, float w, float h, float rx,
                            float ry, const glm::vec4& c);
  uint32_t addfillTriangle(float x1, float y1, float x2, float y2, float x3,
                           float y3, const glm::vec4& c);
  uint32_t addfillPolygon(float x, float y, float xRad, float yRad, float n,
                          const glm::vec4& c);
  uint32_t addfillCircle(float x, float y, float rad, float angleInc,
                         const glm::vec4& c);
  uint32_t addfillEllipse(float x, float y, float xRad, float yRad,
                          float angleInc, const glm::vec4& c);

  //================== Line Primatives ==============//
  uint32_t drawRectangle(float x, float y, float w, float h,
                         const glm::vec4& c);
  uint32_t drawRoundRect(float x, float y, float w, float h, float rx, float ry,
                         const glm::vec4& c);
  uint32_t drawTriangle(float x1, float y1, float x2, float y2, float x3,
                        float y3, const glm::vec4& c);
  uint32_t drawPolygon(const std::vector<float>& v, const glm::vec4& c);
  uint32_t drawPolygon(float x, float y, float xRad, float yRad, float n,
                       const glm::vec4& c);
  uint32_t drawCompletePolygon(float x, float y, float xRad, float yRad,
                               float n, const glm::vec4& c);
  uint32_t drawCircle(float x, float y, float rad, float angleInc,
                      const glm::vec4& c);
  uint32_t drawEllipse(float x, float y, float xRad, float yRad, float angleInc,
                       const glm::vec4& c);

  // TODO: come up with easy API to modify existing StyledMultiShape2Ds...
  void removeSolid(uint32_t index);
  void removeLine(uint32_t index);
};
