#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class MultiTransform{
private:
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;

public:
    MultiTransform(const glm::vec3& pos=glm::vec3(1,1,1), const glm::vec3& rot=glm::vec3(1,1,1),
                const glm::vec3& scale=glm::vec3(1,1,1)) : pos(pos), rot(rot), scale(scale) {}

    void setPos(const glm::vec3 &v){ pos = v; }    

    void setRot(const glm::vec3 &v){ rot = v; }     

    void setScale(const glm::vec3 &v){ scale = v; }

    void incrPos(const glm::vec3 &v){ pos += v; }    

    void incrRot(const glm::vec3 &v){ rot += v; }     

    void incrScale(const glm::vec3 &v){ scale += v; }          
    
    inline glm::mat4 getModel() {
        glm::mat4 rotAll =  glm::rotate(rot.x, glm::vec3(1,0,0))*glm::rotate(rot.y, glm::vec3(0,1,0))*glm::rotate(rot.z, glm::vec3(0,0,1));
        return glm::translate(pos)*rotAll*glm::scale(scale);
    }
};