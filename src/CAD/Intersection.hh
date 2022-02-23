#pragma once
#include "opengl/Shape.hh"
#include "opengl/Canvas.hh"

class Intersection {
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t latRes, longRes, radius;
    public:
        Intersection::Intersection(Canvas* c, Shape shape1, Shape shape2);
        ~Intersection();
        void init();
        void render();
        void cleanup();
};