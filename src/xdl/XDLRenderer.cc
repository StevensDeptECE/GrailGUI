#include "xdl/XDLRenderer.hh"

U32Renderer::U32Renderer() {}
void U32Renderer::display(Buffer& in, Canvas* c, MultiShape2d* m, MultiText* t,
                          float x0, float y0, float* w,
                          float* h) const override {
  StyledMultiShape2D* m = (StyledMultiShape2D*)c->getLayer(0);
  MultiText* t = (MultiText*)c->getLayer(1);
  const Font* f = s->f;
  t->add(x0, y0, f, in.readU32());
}

class U64Renderer : public XDLRenderer {
 public:
  virtual void display(Buffer& in, MultiShape2d* m, MultiText* t, float x0,
                       float y0, float* w, float* h) const override;
};

class F64Renderer : public XDLRenderer {
 public:
  virtual void display(Buffer& in, MultiShape2d* m, MultiText* t, float x0,
                       float y0, float* w, float* h) const override;
};

class VerticalListRenderer : public XDLRenderer {
 public:
  virtual void display(Buffer& in, MultiShape2d* m, MultiText* t, float x0,
                       float y0, float* w, float* h) const override;
};

class ObjectNameValueRenderer : public XDLRenderer {
 public:
  virtual void display(Buffer& in, MultiShape2d* m, MultiText* t, float x0,
                       float y0, float* w, float* h) const override;
};

class ObjectHorizontalRenderer : public XDLRenderer {
 public:
  virtual void display(Buffer& in, MultiShape2d* m, MultiText* t, float x0,
                       float y0, float* w, float* h) const override;
};

void ObjectHorizontalRenderer::display(Buffer& in, MultiShape2d* m,
                                       MultiText* t, float x0, float y0,
                                       float* w, float* h) const {}
