class Renderer {
  private:
    float width; // field width in pixels?
    uint8_t precision; // used for float types, others?
    uint16_t rendererType; // which renderer to call
    Widget2D* w;
  public:
    float renderU8(float x, float y, Buffer& buf) const;
    float renderU16(float x, float y, Buffer& buf) const;
    void renderU32(float x, float y, Buffer& buf) const;
    void renderU64(float x, float y, Buffer& buf) const;
    void renderI8(float x, float y, Buffer& buf) const;
    void renderI16(float x, float y, Buffer& buf) const;
    void renderI32(float x, float y, Buffer& buf) const;
    void renderI64(float x, float y, Buffer& buf) const;
    void renderF32(float x, float y, Buffer& buf) const;
    void renderF64(float x, float y, Buffer& buf) const;
    void renderCString(float x, float y, Buffer& buf) const;
    void renderString(float x, float y, Buffer& buf) const;
    void renderCStringLen(float x, float y, Buffer& buf) const;

    void renderU8Centered(Buffer& buf) const;
    void renderU16Centered(Buffer& buf) const;
    void renderU32Centered(Buffer& buf) const;
    void renderU64Centered(Buffer& buf) const;
    void renderI8Centered(Buffer& buf) const;
    void renderI16Centered(Buffer& buf) const;
    void renderI32Centered(Buffer& buf) const;
    void renderI64Centered(Buffer& buf) const;
    void renderF32Centered(Buffer& buf) const;
    void renderF64Centered(Buffer& buf) const;
    void renderCStringCentered(Buffer& buf) const;
    void renderStringCentered(Buffer& buf) const;
    void renderCStringLenCentered(Buffer& buf) const;

    float renderU8Left(Buffer& buf) const;
    void renderU16Left(Buffer& buf) const;
    void renderU32Left(Buffer& buf) const;
    void renderU64Left(Buffer& buf) const;
    void renderI8Left(Buffer& buf) const;
    void renderI16Left(Buffer& buf) const;
    void renderI32Left(Buffer& buf) const;
    void renderI64Left(Buffer& buf) const;
    void renderF32Left(Buffer& buf) const;
    void renderF64Left(Buffer& buf) const;
    void renderCStringLeft(Buffer& buf) const;
    void renderStringLeft(Buffer& buf) const;
    void renderCStringLenLeft(Buffer& buf) const;

    void renderU8Right(Buffer& buf) const;
    void renderU16Right(Buffer& buf) const;
    void renderU32Right(Buffer& buf) const;
    void renderU64Right(Buffer& buf) const;
    void renderI8Right(Buffer& buf) const;
    void renderI16Right(Buffer& buf) const;
    void renderI32Right(Buffer& buf) const;
    void renderI64Right(Buffer& buf) const;
    void renderF32Right(Buffer& buf) const;
    void renderF64Right(Buffer& buf) const;
    void renderCStringRight(Buffer& buf) const;
    void renderStringRight(Buffer& buf) const;
    void renderCStringLenRight(Buffer& buf) const;

#if 0
    void sparkLine(SparkLine* g, Buffer& buf) const;
    void lineGraph(LineGraph* g, Buffer& buf) const;
    void gapMinder(GapMinder* g, Buffer& buf) const;
    void barChart(BarChartWidget* g, Buffer& buf) const;
    void boxChart(BoxChartWidget* g, Buffer& buf) const;
#endif

}