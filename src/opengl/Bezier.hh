#pragma once
#include <iostream>
#include <glm/glm.hpp>

class Style;
class Bezier{
private:
    float p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y,
          ax,  ay,  bx,  by,  cx,  cy,  dx,  dy;
    int n;
    double dt;
    bool end;
    const glm::vec4& c;
public:
    Bezier(float ax, float bx, float cx, float dx,
           float ay, float by, float cy, float dy, int n, bool end, const glm::vec4& c = {0,0,0,1}) :
           ax(ax), bx(bx), cx(cx), dx(dx), ay(ay), by(by),
           cy(cy), dy(dy), n(n), dt(1.0/n), end(end), c(c){
        p1x = dx;                           p1y = dy;
        p2x = (cx + 3 * dx) / 3;            p2y = (cy + 3 * dy) / 3;
        p3x = (bx - 3 * p1x + 6 * p2x) / 3; p3y = (by - 3 * p1y + 6 * p2y) / 3;
        p4x = ax + p1x - 3 * p2x + 3 * p3x; p4y = ay + p1y - 3 * p2y + 3 * p3y;
    }
    float getax() const{
        return ax;
    }
    float getay() const{
        return ay;
    }
    float getbx() const{
        return bx;
    }
    float getby() const{
        return by;
    }
    float getcx() const{
        return cx;
    }
    float getcy() const{
        return cy;
    }
    float getdx() const{
        return dx;
    }
    float getdy() const{
        return dy;
    }
    float getp1x() const{
        return p1x;
    }
    float getp1y() const{
        return p1y;
    }
    float getp2x() const{
        return p2x;
    }
    float getp2y() const{
        return p2y;
    }
    float getp3x() const{
        return p3x;
    }
    float getp3y() const{
        return p3y;
    }
    float getp4x() const{
        return p4x;
    }
    float getp4y() const{
        return p4y;
    }
    int getn() const{
        return n;
    }
    double getdt() const{
        return dt;
    }
    bool getEnd() const{
        return end;
    }
    glm::vec4 getColor() const{
        return c;
    }
};
