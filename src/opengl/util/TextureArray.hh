#pragma once

#include<iostream>
#include<string>
#include "stb/stb_image.h"
#include "glad/glad.h"
#include "opengl/Shader.hh"

class TextureArray {
public:
    TextureArray(Shader* shader, uint8_t allocSize = 32);
    ~TextureArray();
    // Code from: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
    GLuint loadImage(const char* imagePath, uint8_t unit);
    GLuint bindImage(uint8_t unit);
private:
    Shader* shader;
    uint8_t allocSize;
    GLuint textureID[32]; //TODO: Alex this should be fixed! Maybe a vector? for now, we allocated 32 so we are not out of bounds!!!
};