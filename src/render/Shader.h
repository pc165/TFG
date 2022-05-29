#ifndef TFG_SHADER_H
#define TFG_SHADER_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include "OpenGL.h"
#include "DataStructs.h"

class Shader {
private:
    GLuint programId{};
public:
    Shader() = default;

    [[nodiscard]] GLuint getProgramId() const;

    ~Shader();

    void loadSource(const char *path);


    void setBoolean(const char *name, int ptrData) const {
        auto location = glGetUniformLocation(programId, name);
        glUniform1i(location, ptrData);
    }

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

    void setLight(tfg::Light const &light) const {
        setVec3("uLight.position", glm::value_ptr(light.position));
        setVec3("uLight.direction", glm::value_ptr(light.direction));
        setFloat("uLight.cutOff", light.cutOff);
        setFloat("uLight.outerCutOff", light.outerCutOff);

        setVec3("uLight.ambient", glm::value_ptr(light.ambient));
        setVec3("uLight.diffuse", glm::value_ptr(light.diffuse));
        setVec3("uLight.specular", glm::value_ptr(light.specular));
        setFloat("uLight.constant", light.constant);
        setFloat("uLight.linear", light.linear);
        setFloat("uLight.quadratic", light.quadratic);
    }

    void setMaterial(tfg::Material const &material) const {
        setFloat("uMaterial.shininess", material.shininess);
        setInt("uMaterial.diffuse", material.diffuse);
        setInt("uMaterial.specular", material.specular);
    }

    void bind() const {
        glUseProgram(programId);
    }

    static void unBind() {
        glUseProgram(0);
    }

private:
    static int parseCode(const char *shaderPath, std::string &outCode, std::string const &token);

    static GLuint compileShader(std::string const &code, GLenum type) {
        if (code.empty())
            return 0;

        GLboolean result = GL_FALSE;
        int length;
        GLuint id = glCreateShader(type);

        // Compile Vertex Shader
        char const *vertexCodePtr = code.c_str();
        glShaderSource(id, 1, &vertexCodePtr, nullptr);
        glCompileShader(id);

        // Check Vertex Shader
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        if (length > 0) {
            std::vector<char> msg(length + 1);
            glGetShaderInfoLog(id, length, nullptr, &msg[0]);
            LOG_ERROR("{}", &msg[0]);
            exit(0);
        }

        return id;
    };
};


#endif //TFG_SHADER_H
