#pragma once

#include "opengl/Canvas.hh"

class NewCylinder {
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        float height, radius;
    public:
        Cylinder(float height, float radius);
        //Cylinder(Canvas* c, uint32_t height, uint32_t x, uint32_t y, uint32_t z, uint32_t radius, uint32_t segments);
        //~Cylinder();
        double getVolume();
        void init();
        void render();
        void cleanup();
        void update();
};