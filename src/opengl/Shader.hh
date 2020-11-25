#pragma once

/*
 * Shader encapsulates the ugly GL API for compiling shaders.
 * the private method load() loads shaders of vertex, fragment or geometry types
 * The constructor links these together in a program and saves only the
 * resulting linked ID. This object throws out all scratch data except for the
 * program ID and what is stored on the graphics card. Author: Dov Kruger March
 * 5, 2019
 */

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader {
 private:
  uint32_t progID;
  uint32_t load(const char filename[], uint32_t shaderType,
                const char shaderTypeName[]);
  static std::string shaderDir;
  static std::vector<Shader> shaders;
  static uint32_t format;
  void fastLoad(const char filename[]);
  void save(const char shaderName[]);

 public:
  Shader(const char shaderName[], const char vertexPath[],
         const char fragmentPath[], const char geometryPath[] = nullptr);

  static void setDir(const std::string &shaderDir) {
    Shader::shaderDir = shaderDir;
  }

  static uint32_t load(const char shaderName[], const char vertRelPath[],
                       const char fragRelPath[],
                       const char geomRelPath[] = nullptr);

  static Shader *useShader(uint32_t sh);

  static void cleanAll();
  void cleanup();

  void use() const;
  void setBool(const std::string &name, bool value) const;
  void setBool(const char name[], bool value) const;
  void setBool(int pos, bool value) const;

  void setInt(const std::string &name, int value) const;
  void setInt(const char name[], int value) const;
  void setInt(int pos, int value) const;

  void setFloat(const std::string &name, float value) const;
  void setFloat(const char name[], float value) const;
  void setFloat(int pos, float value) const;

  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const char name[], const glm::vec2 &value) const;
  void setVec2(int pos, const glm::vec2 &value) const;

  void setVec2(const std::string &name, float x, float y) const;
  void setVec2(const char name[], float x, float y) const;
  void setVec2(int pos, float x, float y) const;

  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const char name[], const glm::vec3 &value) const;
  void setVec3(int pos, const glm::vec3 &value) const;

  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec3(const char name[], float x, float y, float z) const;
  void setVec3(int pos, float x, float y, float z) const;

  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const char name[], const glm::vec4 &value) const;
  void setVec4(int pos, const glm::vec4 &value) const;

  void setVec4(const std::string &name, float x, float y, float z, float w);
  void setVec4(const char name[], float x, float y, float z, float w);
  void setVec4(int pos, float x, float y, float z, float w);

  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat2(const char name[], const glm::mat2 &mat) const;
  void setMat2(int pos, const glm::mat2 &mat) const;

  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat3(const char name[], const glm::mat3 &mat) const;
  void setMat3(int pos, const glm::mat3 &mat) const;

  void setMat4(const std::string &name, const glm::mat4 &mat) const;
  void setMat4(const char name[], const glm::mat4 &mat) const;
  void setMat4(int pos, const glm::mat4 &mat) const;

  uint32_t getID() const { return progID; }
};
