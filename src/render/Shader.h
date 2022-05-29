#ifndef TFG_SHADER_H
#define TFG_SHADER_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include "OpenGL.h"

enum ShaderType {
    Vertex, Fragment
};

class Shader {
private:
    std::string vertexCode{};
    std::string fragmentCode{};
    GLuint programId{};
public:
    Shader() = default;

    [[nodiscard]] GLuint getProgramId() const;

    ~Shader();

    void loadSource(const char *path);

    void setMat4(const char *name, const float *ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, ptrData);
    }

    void setMat3(const char *name, const float *ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniformMatrix3fv(location, 1, GL_FALSE, ptrData);
    }

    void setVec4(const char *name, const float *ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniform4fv(location, 1, ptrData);
    }

    void setVec3(const char *name, const float *ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniform3fv(location, 1, ptrData);
    }

    void setVec2(const char *name, const float *ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniform2fv(location, 1, ptrData);
    }

    void setFloat(const char *name, float ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniform1f(location, ptrData);
    }

    void setInt(const char *name, int ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniform1i(location, ptrData);
    }

    void bind() const {
        glUseProgram(programId);
    }

    static void unBind() {
        glUseProgram(0);
    }

private:
    static int parseCode(const char *shaderPath, std::string &outCode, const ShaderType &shaderType);

    static GLuint compileSource(const std::string &vertexCode, const std::string &fragmentCode);
};


#endif //TFG_SHADER_H
