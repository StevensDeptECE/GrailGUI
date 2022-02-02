#pragma once
#include "opengl/Shape.hh"

class Sphere {
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t latRes, longRes, radius;
    public:
        Sphere::Sphere(uint32_t latRes, uint32_t longRes, uint32_t radius);
        ~Sphere();
        void init();
        void render();
        void cleanup();
};