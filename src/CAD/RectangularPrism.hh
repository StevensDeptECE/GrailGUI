#pragma once

#include "opengl/Shape.hh"
#include "opengl/Canvas.hh"

class RectangularPrism : public Shape {
    private:
        std::vector<uint32_t> vert;
        std::vector<uint32_t> ind;
        void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);
        uint32_t length, width, height, x, y, z;
        uint32_t elementbuffer;
    public:
        RectangularPrism(Canvas* c, uint32_t length, uint32_t width, uint32_t height, uint32_t x, uint32_t y, uint32_t z);
        //~RectangularPrism();
        double getVolume();
        void init();
        void render();
        void cleanup();
        void update();
};