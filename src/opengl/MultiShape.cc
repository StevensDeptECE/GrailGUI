#include "opengl/MultiShape.hh"

MultiShape::~MultiShape() {}

void MultiShape::process_input(Inputs* in, float dt) {}

void MultiShape::update() {}

void MultiShape::sAddTriIndices(const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - 3;
  solidIndices.push_back(startIndex);
  solidIndices.push_back(startIndex + 1);
  solidIndices.push_back(startIndex + 2);
}

void MultiShape::sAddQuadIndices(const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - 4;
  solidIndices.push_back(startIndex);
  solidIndices.push_back(startIndex + 1);
  solidIndices.push_back(startIndex + 2);

  solidIndices.push_back(startIndex + 2);
  solidIndices.push_back(startIndex + 3);
  solidIndices.push_back(startIndex);
}

void MultiShape::sAddSectorIndices(uint32_t centerIndex, uint32_t indexCount,
                                   const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - indexCount;
  for (uint32_t curIndex = startIndex; curIndex < startIndex + indexCount - 1;
       curIndex++) {
    solidIndices.push_back(centerIndex);
    solidIndices.push_back(curIndex);
    solidIndices.push_back(curIndex + 1);
  }
}

void MultiShape::lAddTriIndices(const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - 3;
  lineIndices.push_back(startIndex);
  lineIndices.push_back(startIndex + 1);
  lineIndices.push_back(startIndex + 1);
  lineIndices.push_back(startIndex + 2);
  lineIndices.push_back(startIndex + 2);
  lineIndices.push_back(startIndex);
}

void MultiShape::lAddQuadIndices(const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - 4;
  lineIndices.push_back(startIndex);
  lineIndices.push_back(startIndex + 1);
  lineIndices.push_back(startIndex + 1);
  lineIndices.push_back(startIndex + 2);
  lineIndices.push_back(startIndex + 2);
  lineIndices.push_back(startIndex + 3);
  lineIndices.push_back(startIndex + 3);
  lineIndices.push_back(startIndex);
}

void MultiShape::lAddSectorIndices(uint32_t centerIndex, uint32_t indexCount,
                                   const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - indexCount;
  for (uint32_t curIndex = startIndex + 1;
       curIndex < startIndex + indexCount - 1; curIndex++) {
    lineIndices.push_back(curIndex);
    lineIndices.push_back(curIndex + 1);
  }
}

void MultiShape::pAddTriIndices(const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - 3;

  pointIndices.push_back(startIndex);
  pointIndices.push_back(startIndex + 1);
  pointIndices.push_back(startIndex + 2);
}

void MultiShape::pAddQuadIndices(const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - 4;

  pointIndices.push_back(startIndex);
  pointIndices.push_back(startIndex + 1);
  pointIndices.push_back(startIndex + 2);
  pointIndices.push_back(startIndex + 3);
}

void MultiShape::pAddSectorIndices(uint32_t centerIndex, uint32_t indexCount,
                                   const uint32_t elemPerVert) {
  uint32_t startIndex = getPointIndex(elemPerVert) - indexCount;
  for (uint32_t curIndex = startIndex; curIndex < startIndex + indexCount - 1;
       curIndex++) {
    pointIndices.push_back(curIndex);
  }
}
