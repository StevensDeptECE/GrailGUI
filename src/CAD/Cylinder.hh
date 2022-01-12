#pragma once
#include "opengl/Shape.hh"

class Cylinder {
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t height, radius, x, y, segments;
    public:
        Cylinder(uint32_t height, uint32_t r1, uint32_t r2);
        Cylinder::Cylinder(uint32_t height, uint32_t x, uint32_t y, uint32_t radius, uint32_t segments, const Style* s);
        ~Cylinder();
        double getVolume();
        void init();
        void render();
        void cleanup();
};