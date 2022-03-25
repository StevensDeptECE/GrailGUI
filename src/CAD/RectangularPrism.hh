#pragma once
#include "opengl/Shape.hh"

class RectangularPrism : public Shape{
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);
    public:
        RectangularPrism(uint32_t length, uint32_t width, uint32_t height);
        ~RectangularPrism();
        double getVolume();
        void init();
        void render();
        void cleanup();
};