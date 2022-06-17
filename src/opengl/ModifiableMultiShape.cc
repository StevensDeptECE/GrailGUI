#include "opengl/ModifiableMultiShape.hh"

uint32_t ModifiableMultiShape::addfillRectangle(float x, float y, float w,
                                                float h, const glm::vec4& c) {
  uint32_t index = solidList.size();
  uint32_t startVertexIndex = vertices.size();
  uint32_t startSolidIndex = solidIndices.size();
  uint32_t points = 4;
  uint32_t numInVertices = elemPerVert * points;
  solidList.add(
      SolidInfo(startVertexIndex, points, startSolidIndex, numInVertices));
  // change StyledMultiShape2d so if you are bigger than the original size,
  // reallocate
  StyledMultiShape2D::fillRectangle(x, y, w, h, c);
  tab->getParentWin()->setUpdate();
  return index;  // solidList.size() - 1;
}

void ModifiableMultiShape::removeSolid(uint32_t index) {
  auto startVertex = vertices.begin() + solidList[index].startVertexIndex;
  vertices.erase(startVertex, startVertex + solidList[index].numInVertices);

  auto startIndex = solidIndices.begin() + solidList[index].startSolidIndex;
  solidIndices.erase(startIndex, startIndex + 6);

  for (auto i = startIndex; i != solidIndices.end(); ++i) {
    *i -= 4;  // TODO:only for rectangles also fix indices
  }           // tab->getParentWin()->setUpdate();
}