#pragma once
#include "opengl/Shape.hh"

class Sphere {
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t radius, x, y, z;
    public:
        Sphere::Sphere(uint32_t radius, uint32_t x, uint32_t y, uint32_t z);
        ~Sphere();
        void init();
        void render();
        void cleanup();
};