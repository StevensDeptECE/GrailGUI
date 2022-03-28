#pragma once
#include "opengl/Shape.hh"

class Cylinder : public Shape{
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t height, r1, r2;

       public:
        Cylinder(Canvas* parent, uint32_t height, uint32_t r1, uint32_t r2);
        Cylinder(Canvas* parent, uint32_t height, uint32_t r1);
        ~Cylinder();
        double getVolume();
        void init();
        void render();
        void cleanup();
};