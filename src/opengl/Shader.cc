#include "opengl/Shader.hh"

#include <fstream>
#include <sstream>
using namespace std;
#include <glad/glad.h>

#include "util/Ex.hh"
#include "util/Prefs.hh"
vector<Shader> Shader::shaders;
uint32_t Shader::format;
string Shader::shaderDir;

inline uint32_t Shader::load(const char filename[], GLuint shaderType,
                             const char shaderTypeName[]) {
  ifstream shaderFile(filename);
  if (!shaderFile) throw "Failed to load shader";

  stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  shaderFile.close();

  string shaderCode = shaderStream.str();
  uint32_t id = glCreateShader(shaderType);
  const char *sourceCode = shaderCode.c_str();
  glShaderSource(id, 1, &sourceCode, nullptr);
  glCompileShader(id);

  GLint success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar infoLog[1024];
    glGetShaderInfoLog(id, 1024, NULL, infoLog);
    cout << "Error compiling shader: " << shaderTypeName << "\n"
         << infoLog
         << "\n -- --------------------------------------------------- --\n";
  }
  return id;
}

void Shader::fastLoad(const char filename[]) {
  progID = glCreateProgram();
  ifstream inputStream(filename, std::ios::binary);
  istreambuf_iterator<char> startIt(inputStream), endIt;
  vector<char> buffer(startIt, endIt);  // Load file
  inputStream.close();

  // Install shader binary
  glProgramBinary(progID, format, buffer.data(), buffer.size());

  // Check for success/failure
  GLint status;
  glGetProgramiv(progID, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    cerr << "failed to load GLSL program back\n";
  }
}

void Shader::save(const char shaderName[]) {
  GLint formats = 0;
  glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
  if (formats < 1) {
    prefs.trySavingShader = false;  // if not supported, never try again
    cerr << "Driver does not support any binary formats.\n";
    return;
  }
  // Get the binary length
  GLint length = 0;
  glGetProgramiv(progID, GL_PROGRAM_BINARY_LENGTH, &length);

  // Retrieve the binary code
  std::vector<GLubyte> buffer(length);
  glGetProgramBinary(progID, length, nullptr, &format, buffer.data());

  // Write the binary to a file.
  cout << "Writing to " << shaderName << ", binary format = " << format
       << std::endl;
  std::ofstream out(shaderName, std::ios::binary);
  out.write(reinterpret_cast<char *>(buffer.data()), length);
  out.close();
  prefs.setFastLoadShaders(format);
}

Shader::Shader(const char shaderName[], const char vertexPath[],
               const char fragmentPath[], const char geometryPath[]) {
  if (vertexPath == nullptr || fragmentPath == nullptr) return;
  try {
    // if (prefs.getFastLoadShaders()) {
    //   fastLoad(shaderName);
    //   return;
    // }
    uint32_t vertexShaderID =
        load(vertexPath, GL_VERTEX_SHADER, "VERTEX");  // load the vertex shader
    uint32_t fragmentShaderID = load(fragmentPath, GL_FRAGMENT_SHADER,
                                     "FRAGMENT");  // load the fragment shader
    uint32_t geometryShaderID;
    progID = glCreateProgram();
    glAttachShader(progID, vertexShaderID);
    glAttachShader(progID, fragmentShaderID);
    // if geometry shader path is present, also load a geometry shader
    if (geometryPath != nullptr) {
      geometryShaderID = load(geometryPath, GL_GEOMETRY_SHADER, "GEOMETRY");
      glAttachShader(progID, geometryShaderID);
    }
    GLint success;
    GLchar infoLog[1024];
    glLinkProgram(progID);
    glGetProgramiv(progID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(progID, 1024, NULL, infoLog);
      cout << "Error linking shader:\n"
           << infoLog
           << "\n -- --------------------------------------------------- --\n";
    }

    // delete the shaders as they're linked in now and no longer necessary
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    if (geometryPath != nullptr) glDeleteShader(geometryShaderID);
    //		if (prefs.trySavingShader)
    // save(shaderName);
  } catch (const ifstream::failure &e) {
    cout << "HI  "
         << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
  }
}

Shader *Shader::useShader(uint32_t sh) {
  shaders[sh].use();
  if (sh >= shaders.size())
    throw Ex2(Errcode::MISSING_SHADER, std::to_string(sh));
  return &shaders[sh];
}

uint32_t Shader::load(const char shaderName[], const char vertRelPath[],
                      const char fragRelPath[], const char geomRelPath[]) {
  string vertPath = shaderDir + vertRelPath;
  string fragPath = shaderDir + fragRelPath;
  //	string geomPath =
  // equivalent to push_back(Shader(...))
  shaders.emplace_back(shaderName, vertPath.c_str(), fragPath.c_str(), nullptr);
  return shaders.size() - 1;
}

void Shader::cleanup() { glDeleteProgram(progID); }

void Shader::cleanAll() {
  for (auto &s : shaders) {
    s.cleanup();
  }
}

void Shader::use() const { glUseProgram(progID); }

void Shader::setBool(const string &name, bool value) const {
  glUniform1i(glGetUniformLocation(progID, name.c_str()), int(value));
}

void Shader::setBool(const char name[], bool value) const {
  glUniform1i(glGetUniformLocation(progID, name), int(value));
}

void Shader::setBool(int pos, bool value) const {
  glUniform1i(pos, int(value));
}

void Shader::setInt(const string &name, int value) const {
  glUniform1i(glGetUniformLocation(progID, name.c_str()), value);
}

void Shader::setInt(const char name[], int value) const {
  glUniform1i(glGetUniformLocation(progID, name), value);
}

void Shader::setInt(int pos, int value) const { glUniform1i(pos, value); }

void Shader::setFloat(const string &name, float value) const {
  glUniform1f(glGetUniformLocation(progID, name.c_str()), value);
}

void Shader::setFloat(const char name[], float value) const {
  glUniform1f(glGetUniformLocation(progID, name), value);
}

void Shader::setFloat(int pos, float value) const { glUniform1f(pos, value); }

void Shader::setVec2(const string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(progID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const char name[], const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(progID, name), 1, &value[0]);
}

void Shader::setVec2(int pos, const glm::vec2 &value) const {
  glUniform2fv(pos, 1, &value[0]);
}

void Shader::setVec2(const string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(progID, name.c_str()), x, y);
}

void Shader::setVec2(const char name[], float x, float y) const {
  glUniform2f(glGetUniformLocation(progID, name), x, y);
}

void Shader::setVec2(int pos, float x, float y) const {
  glUniform2f(pos, x, y);
}

void Shader::setVec3(const string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(progID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const char name[], const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(progID, name), 1, &value[0]);
}

void Shader::setVec3(int pos, const glm::vec3 &value) const {
  glUniform3fv(pos, 1, &value[0]);
}

void Shader::setVec3(const string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(progID, name.c_str()), x, y, z);
}

void Shader::setVec3(const char name[], float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(progID, name), x, y, z);
}

void Shader::setVec3(int pos, float x, float y, float z) const {
  glUniform3f(pos, x, y, z);
}

void Shader::setVec4(const string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(progID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const char name[], const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(progID, name), 1, &value[0]);
}

void Shader::setVec4(int pos, const glm::vec4 &value) const {
  glUniform4fv(pos, 1, &value[0]);
}

void Shader::setVec4(const string &name, float x, float y, float z, float w) {
  glUniform4f(glGetUniformLocation(progID, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const char name[], float x, float y, float z, float w) {
  glUniform4f(glGetUniformLocation(progID, name), x, y, z, w);
}

void Shader::setVec4(int pos, float x, float y, float z, float w) {
  glUniform4f(pos, x, y, z, w);
}

void Shader::setMat2(const string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat2(const char name[], const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(progID, name), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat2(int pos, const glm::mat2 &mat) const {
  glUniformMatrix2fv(pos, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat3(const char name[], const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(progID, name), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat3(int pos, const glm::mat3 &mat) const {
  glUniformMatrix3fv(pos, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat4(const char name[], const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(progID, name), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat4(int pos, const glm::mat4 &mat) const {
  glUniformMatrix4fv(pos, 1, GL_FALSE, &mat[0][0]);
}
