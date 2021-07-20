class Buffer;

class Renderer {
 private:
  float width;  // field width in pixels?
  float height;
  uint8_t digits;
  uint8_t precision;      // used for float types, others?
  uint16_t rendererType;  // which renderer to call
  Font* f;
  StyledMultiShape2D* m;
  MultiText* t;
  Widget2D* w;

 public:
  float renderU8(float x, float y, Buffer& buf) const;
  float renderU16(float x, float y, Buffer& buf) const;
  float renderU32(float x, float y, Buffer& buf) const;
  float renderU64(float x, float y, Buffer& buf) const;
  float renderI8(float x, float y, Buffer& buf) const;
  float renderI16(float x, float y, Buffer& buf) const;
  float renderI32(float x, float y, Buffer& buf) const;
  float renderI64(float x, float y, Buffer& buf) const;
  float renderF32(float x, float y, Buffer& buf) const;
  float renderF64(float x, float y, Buffer& buf) const;
  float renderCString(float x, float y, Buffer& buf) const;
  float renderString(float x, float y, Buffer& buf) const;
  float renderCStringLen(float x, float y, Buffer& buf) const;
  float renderDate(float x, float y, Buffer& buf) const;

  float renderU8Centered(float x, float y, Buffer& buf) const;
  float renderU16Centered(float x, float y, Buffer& buf) const;
  float renderU32Centered(float x, float y, Buffer& buf) const;
  float renderU64Centered(float x, float y, Buffer& buf) const;
  float renderI8Centered(float x, float y, Buffer& buf) const;
  float renderI16Centered(float x, float y, Buffer& buf) const;
  float renderI32Centered(float x, float y, Buffer& buf) const;
  float renderI64Centered(float x, float y, Buffer& buf) const;
  float renderF32Centered(float x, float y, Buffer& buf) const;
  float renderF64Centered(float x, float y, Buffer& buf) const;
  float renderStringCentered(float x, float y, Buffer& buf) const;
  float renderDateCentered(float x, float y, Buffer& buf) const;

  float renderU8Left(float x, float y, Buffer& buf) const;
  float renderU16Left(float x, float y, Buffer& buf) const;
  float renderU32Left(float x, float y, Buffer& buf) const;
  float renderU64Left(float x, float y, Buffer& buf) const;
  float renderI8Left(float x, float y, Buffer& buf) const;
  float renderI16Left(float x, float y, Buffer& buf) const;
  float renderI32Left(float x, float y, Buffer& buf) const;
  float renderI64Left(float x, float y, Buffer& buf) const;
  float renderF32Left(float x, float y, Buffer& buf) const;
  float renderF64Left(float x, float y, Buffer& buf) const;
  float renderStringLeft(float x, float y, Buffer& buf) const;

  float renderU8Right(float x, float y, Buffer& buf) const;
  float renderU16Right(float x, float y, Buffer& buf) const;
  float renderU32Right(float x, float y, Buffer& buf) const;
  float renderU64Right(float x, float y, Buffer& buf) const;
  float renderI8Right(float x, float y, Buffer& buf) const;
  float renderI16Right(float x, float y, Buffer& buf) const;
  float renderI32Right(float x, float y, Buffer& buf) const;
  float renderI64Right(float x, float y, Buffer& buf) const;
  float renderF32Right(float x, float y, Buffer& buf) const;
  float renderF64Right(float x, float y, Buffer& buf) const;
  float renderStringRight(float x, float y, Buffer& buf) const;

#if 0
    float sparkLine(SparkLine* g, Buffer& buf) const;
    float lineGraph(LineGraph* g, Buffer& buf) const;
    float gapMinder(GapMinder* g, Buffer& buf) const;
    float barChart(BarChartWidget* g, Buffer& buf) const;
    float boxChart(BoxChartWidget* g, Buffer& buf) const;
#endif
}