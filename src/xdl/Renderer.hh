#include "opengl/Errcode.hh"
#include "xdl/std.hh"

/*
	abstract class for any object that can take an XDL object and draw it
	in OpenGL
 */
class Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const = 0;
	enum Dir { UP, DOWN, LEFT, RIGHT};
	void update(Dir);
};

class U32Renderer : public Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const override;
};

class U64Renderer : public Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const override;
};

class F64Renderer : public Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const override;
};

class VerticalListRenderer : public Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const override;
};

class ObjectNameValueRenderer : public Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const override;
};

class ObjectHorizontalRenderer : public Renderer {
public:
	virtual void display(Buffer& in, MultiShape2d* m, MultiText* t,
											 float x0, float y0, float* w, float* h) const override;
};







