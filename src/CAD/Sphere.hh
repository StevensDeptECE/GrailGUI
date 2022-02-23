#pragma once
#include "opengl/Shape.hh"
<<<<<<< HEAD
#include "opengl/Canvas.hh"
class Sphere : Shape{
=======

class Sphere : public Shape {
>>>>>>> 644dcecb0ce6959ef8b5a74af10b4a0cdca57c9a
    private:
        std::vector<float> vert;
        std::vector<uint32_t> ind;
        uint32_t latRes, longRes, radius;
    public:
<<<<<<< HEAD
        Sphere::Sphere(Canvas* c, uint32_t latRes, uint32_t longRes, uint32_t radius);
=======
        Sphere(Canvas* c, uint32_t latRes, uint32_t longRes, uint32_t radius);
>>>>>>> 644dcecb0ce6959ef8b5a74af10b4a0cdca57c9a
        ~Sphere();
        void init();
        void render();
        void cleanup();
        void update();
};