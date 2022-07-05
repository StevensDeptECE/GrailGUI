#include "opengl/Image.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "util/DynArray.hh"

class ModifiableMultiShape : public StyledMultiShape2D {
 private:
  struct SolidInfo {
    uint32_t startVertexIndex;
    uint32_t numInIndices;  // starting position within vertices // how many //
                            // floating point numbers does this use?
    uint32_t startSolidIndex;  // start position in sbo (nice)
    uint32_t numInVertices;
    uint32_t shapeIndices;
  };

  struct LineInfo {
    uint32_t lStartVertexIndex;
    uint32_t lnumInIndices;   // starting position within vertices
    uint32_t startLineIndex;  // how many floating point numbers does this use?
    uint32_t lnumInVertices;  // start position in lbo
    uint32_t lShapeIndices;
  };
  std::vector<SolidInfo> solidList;
  std::vector<LineInfo> lineList;
  uint32_t index;
  Tab* tab;

 public:
  // create a rectangle, returning a handle to delete it if needed?

  ModifiableMultiShape(Canvas* c, const Style* s, Tab* tab,
                       uint32_t solidListCount = 1024,
                       uint32_t lineListCount = 1024)
      : StyledMultiShape2D(c, s), tab(tab) {
    solidList.reserve(solidListCount);
    lineList.reserve(lineListCount);
  };

  //====================== Util Functions =================//
  void updateSolidList(uint32_t startVertexIndex, uint32_t startSolidIndex);
  void updateLineList(uint32_t lStartVertexIndex, uint32_t startLineIndex);

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
  uint32_t adddrawRectangle(float x, float y, float w, float h,
                            const glm::vec4& c);
  uint32_t adddrawRoundRect(float x, float y, float w, float h, float rx,
                            float ry, const glm::vec4& c);
  uint32_t adddrawTriangle(float x1, float y1, float x2, float y2, float x3,
                           float y3, const glm::vec4& c);
  uint32_t adddrawPolygon(float x, float y, float xRad, float yRad, float n,
                          const glm::vec4& c);
  uint32_t adddrawCircle(float x, float y, float rad, float angleInc,
                         const glm::vec4& c);
  uint32_t adddrawEllipse(float x, float y, float xRad, float yRad,
                          float angleInc, const glm::vec4& c);

  // TODO: come up with easy API to modify existing StyledMultiShape2Ds...
  void removeSolid(uint32_t index);
  void removeLine(uint32_t index);
};
