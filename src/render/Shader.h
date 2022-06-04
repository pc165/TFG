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
    std::unordered_map<std::string, int> location;
public:
    Shader() = default;

    [[nodiscard]] GLuint getProgramId() const;

    ~Shader();

    void loadSource(const char *path);

    int getLocation(std::string const &name) const {
        auto a = location.find(name);
        assert(a != location.end());
        return a->second;
    }

    void setBoolean(std::string const &name, int ptrData) const {
        glUniform1i(getLocation(name), ptrData);
    }

    void setMat4(std::string const &name, const float *ptrData) const {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, ptrData);
    }

    void setMat3(std::string const &name, const float *ptrData) const {
        glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, ptrData);
    }

    void setVec4(std::string const &name, const float *ptrData) const {
        glUniform4fv(getLocation(name), 1, ptrData);
    }

    void setVec3(std::string const &name, const float *ptrData) const {
        glUniform3fv(getLocation(name), 1, ptrData);
    }

    void setVec2(std::string const &name, const float *ptrData) const {
        glUniform2fv(getLocation(name), 1, ptrData);
    }

    void setFloat(std::string const &name, float ptrData) const {
        glUniform1f(getLocation(name), ptrData);
    }

    void setInt(std::string const &name, int ptrData) const {
        glUniform1i(getLocation(name), ptrData);
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
