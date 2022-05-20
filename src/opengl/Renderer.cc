#include "opengl/Renderer.hh"

#include <string>

#include "util/Buffer.hh"
using namespace std;

float Renderer::renderU8(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readU8());
}

float Renderer::renderU16(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readU16());
}

float Renderer::renderU32(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readU32());
}

float Renderer::renderU64(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readU64());
}
float Renderer::renderI8(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readI8());
}
float Renderer::renderI16(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readI16());
}
float Renderer::renderI32(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readI32());
}
float Renderer::renderI64(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readI64());
}
float Renderer::renderF32(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readF32());
}
float Renderer::renderF64(float x, float y, Buffer& buf) const {
  return t->addx(x, y, f, buf.readF64());
}

float Renderer::renderString(float x, float y, Buffer& buf) const {
  string s = buf.readString8();
  return t->add(x, y, f, s);
}

float Renderer::renderDate(float x, float y, Buffer& buf) const { return 0; }

float Renderer::renderU8Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readU8());
  return x + width;  // return the position of the next field
}
float Renderer::renderU16Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readU16());
  return x + width;  // return the position of the next field
}

float Renderer::renderU32Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readU32());
  return x + width;  // return the position of the next field
}

float Renderer::renderU64Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readU64());
  return x + width;  // return the position of the next field
}
float Renderer::renderI8Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readI8());
  return x + width;
}
float Renderer::renderI16Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readI16());
  return x + width;
}
float Renderer::renderI32Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readI32());
  return x + width;
}
float Renderer::renderI64Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readI64());
  return x + width;
}
float Renderer::renderF32Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readF32());
  return x + width;
}
float Renderer::renderF64Centered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, width, f, buf.readF64());
  return x + width;
}
float Renderer::renderStringCentered(float x, float y, Buffer& buf) const {
  t->addCentered(x, y, f, buf.readString8());
  return x + width;
}
float Renderer::renderDateCentered(float x, float y, Buffer& buf) const {
  // TODO: impl readDate
  t->addCentered(x, y, f, buf.readI32());
  return x + width;
}

float Renderer::renderU8Left(float x, float y, Buffer& buf) const {
  t->addx(x, y, f, buf.readU8());
  return x + width;  // return the position of the next field
}
float Renderer::renderU16Left(float x, float y, Buffer& buf) const {
  t->addx(x, y, f, buf.readU16());
  return x + width;  // return the position of the next field
}
float Renderer::renderU32Left(float x, float y, Buffer& buf) const {
  t->addx(x, y, f, buf.readU32());
  return x + width;  // return the position of the next field
}

float Renderer::renderU64Left(float x, float y, Buffer& buf) const {
  t->addx(x, y, f, buf.readU64());
  return x + width;  // return the position of the next field
}

float Renderer::renderI8Left(float x, float y, Buffer& buf) const {}
float Renderer::renderI16Left(float x, float y, Buffer& buf) const {}
float Renderer::renderI32Left(float x, float y, Buffer& buf) const {}
float Renderer::renderI64Left(float x, float y, Buffer& buf) const {}
float Renderer::renderF32Left(float x, float y, Buffer& buf) const {}
float Renderer::renderF64Left(float x, float y, Buffer& buf) const {}
float Renderer::renderStringLeft(float x, float y, Buffer& buf) const {}

float Renderer::renderU8Right(float x, float y, Buffer& buf) const {}
float Renderer::renderU16Right(float x, float y, Buffer& buf) const {}
float Renderer::renderU32Right(float x, float y, Buffer& buf) const {}
float Renderer::renderU64Right(float x, float y, Buffer& buf) const {}
float Renderer::renderI8Right(float x, float y, Buffer& buf) const {}
float Renderer::renderI16Right(float x, float y, Buffer& buf) const {}
float Renderer::renderI32Right(float x, float y, Buffer& buf) const {}
float Renderer::renderI64Right(float x, float y, Buffer& buf) const {}
float Renderer::renderF32Right(float x, float y, Buffer& buf) const {}
float Renderer::renderF64Right(float x, float y, Buffer& buf) const {}
float Renderer::renderStringRight(float x, float y, Buffer& buf) const {}

#if 0
    float sparkLine(SparkLine* g, Buffer& buf) const {}
    float lineGraph(LineGraph* g, Buffer& buf) const {}
    float gapMinder(GapMinder* g, Buffer& buf) const {}
    float barChart(BarChartWidget* g, Buffer& buf) const {}
    float boxChart(BoxChartWidget* g, Buffer& buf) const {}
#endif
