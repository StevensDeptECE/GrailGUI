#pragma once
#include "opengl/Shape.hh"

class Cylinder : public Shape{
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
    public:
        Cylinder(uint32_t height, uint32_t r1, uint32_t r2);
        Cylinder(uint32_t height, uint32_t r1);
        ~Cylinder();
        double getVolume();
        void init();
        void render();
        void cleanup();
};