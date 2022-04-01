#pragma once
#include "opengl/Shape.hh"
//#include "opengl/Style.hh"

class Style;

class Sphere : public Shape {
    private:
        const Style* style;
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t latRes, longRes, radius;
    public:
        Sphere(Canvas* c, const Style *s, uint32_t latRes, uint32_t longRes, uint32_t radius);
        ~Sphere();
        void init();
        void render();
        void cleanup();
        void update();
};