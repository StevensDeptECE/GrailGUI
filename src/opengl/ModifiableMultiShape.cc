#include "opengl/ModifiableMultiShape.hh"

// ===========================Util Functions==========================//
void ModifiableMultiShape::updateSolidList(uint32_t startVertexIndex,
                                           uint32_t startSolidIndex) {
  uint32_t points =
      numIndices.back();  // numIndices is in StyledMultiShape2d
                          // refers to number of points, get the latest
  uint32_t CurrentShapeIndices = IndiceCount.back();
  uint32_t numInVertices = elemPerVert * points;
  solidList.push_back(SolidInfo(startVertexIndex, points, startSolidIndex,
                                numInVertices, CurrentShapeIndices));
}

/*void ModifiableMultiShape::UpdateLineList(uint32_t lStartVertexIndex,
                                          uint32_t startLineIndex) {
  uint32_t points =
      numIndices.back();  // numIndices is in StyledMultiShape2d
                          // refers to number of points, get the latest
  uint32_t CurrentShapeIndices = IndiceCount.back();
  uint32_t numInVertices = elemPerVert * points;
  solidList.add(SolidInfo(lStartVertexIndex, points, startLineIndex,
                          numInVertices, CurrentShapeIndices));
}*/

// ===========================solid Primatives==============================///

uint32_t ModifiableMultiShape::addfillRectangle(float x, float y, float w,
                                                float h, const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  StyledMultiShape2D::fillRectangle(x, y, w, h, c);
  updateSolidList(startVertexIndex, startSolidIndex);
  return index;  // solidList.size() - 1;
}

uint32_t ModifiableMultiShape::addfillRoundRect(float x, float y, float w,
                                                float h, float rx, float ry,
                                                const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  StyledMultiShape2D::fillRoundRect(x, y, w, h, rx, ry, c);
  updateSolidList(startVertexIndex, startSolidIndex);
  return index;  // solidList.size() - 1;
}

uint32_t ModifiableMultiShape::addfillTriangle(float x1, float y1, float x2,
                                               float y2, float x3, float y3,
                                               const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  StyledMultiShape2D::fillTriangle(x1, y1, x2, y2, x3, y3, c);
  updateSolidList(startVertexIndex, startSolidIndex);
  return index;  // solidList.size() - 1;
}

uint32_t ModifiableMultiShape::addfillPolygon(float x, float y, float xRad,
                                              float yRad, float n,
                                              const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  StyledMultiShape2D::fillPolygon(x, y, xRad, yRad, n, c);
  updateSolidList(startVertexIndex, startSolidIndex);
  return index;  // solidList.size() - 1;
}

uint32_t ModifiableMultiShape::addfillCircle(float x, float y, float rad,
                                             float angleInc,
                                             const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  StyledMultiShape2D::fillCircle(x, y, rad, angleInc, c);
  updateSolidList(startVertexIndex, startSolidIndex);
  return index;  // solidList.size() - 1;
}

uint32_t ModifiableMultiShape::addfillEllipse(float x, float y, float xRad,
                                              float yRad, float angleInc,
                                              const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  StyledMultiShape2D::fillEllipse(x, y, xRad, yRad, angleInc, c);
  updateSolidList(startVertexIndex, startSolidIndex);
  return index;  // solidList.size() - 1;
}

//====================Line Primatives=================//

//=====================Remove Functions================//

void ModifiableMultiShape::removeSolid(uint32_t index) {
  auto startVertex = vertices.begin() + solidList[index].startVertexIndex;
  vertices.erase(startVertex, startVertex + solidList[index].numInVertices);

  auto startIndex = solidIndices.begin() + solidList[index].startSolidIndex;
  solidIndices.erase(startIndex, startIndex + solidList[index].shapeIndices);

  for (auto i = solidList.begin() + index + 1; i != solidList.end(); ++i) {
    (*i).startVertexIndex -= solidList[index].numInVertices;
  }  // fix index for vertices

  for (auto i = solidList.begin() + index + 1; i != solidList.end(); ++i) {
    (*i).startSolidIndex -= solidList[index].shapeIndices;
  }  // fix index for indices

  for (auto i = startIndex; i != solidIndices.end(); ++i) {
    *i -= solidList[index]
              .numInIndices;  // TODO:only for rectangles also fix indices
  }
}

/*void ModifiableMultiShape::removeLine(uint32_t index) {
  auto startVertex = vertices.begin() + lineList[index].startVertexIndex;
  vertices.erase(startVertex, startVertex + solidList[index].numInVertices);

  auto startIndex = solidIndices.begin() + solidList[index].startSolidIndex;
  solidIndices.erase(startIndex, startIndex + solidList[index].ShapeIndices);

  for (auto i = startIndex; i != solidIndices.end(); ++i) {
    *i -= solidList[index]
              .numInIndices;  // TODO:only for rectangles also fix indices
  }                      // tab->getParentWin()->setUpdate();
}*/