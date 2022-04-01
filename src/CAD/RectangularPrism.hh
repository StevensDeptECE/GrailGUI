#pragma once

#include "opengl/Shape.hh"

class Style;

class RectangularPrism : public Shape {
    private:
        const Style* style;
        std::vector<float> vert;
        std::vector<float> ind;
        void addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);
        float length, width, height, x, y, z;
        float elementbuffer;
        void add3DPoint(float x, float y, float z) {
            vert.push_back(x);
            vert.push_back(y);
            vert.push_back(z);
        }
    public:
        RectangularPrism(Canvas* c, const Style *s, float length, float width, float height, float x, float y, float z);
        ~RectangularPrism();
        double getVolume();
        void init();
        void render();
        void cleanup();
        void update();
};