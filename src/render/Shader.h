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

    int getLocation(std::string const &name) const;

    void setBoolean(std::string const &name, int ptrData) const;

    void setMat4(std::string const &name, const float *ptrData) const;

    void setMat3(std::string const &name, const float *ptrData) const;

    void setVec4(std::string const &name, const float *ptrData) const;

    void setVec3(std::string const &name, const float *ptrData) const;

    void setVec2(std::string const &name, const float *ptrData) const;

    void setFloat(std::string const &name, float ptrData) const;

    void setInt(std::string const &name, int ptrData) const;

    void setLight(tfg::Light const &light) const;

    void setMaterial(tfg::Material const &material) const;

    void bind() const;

    static void unBind();

private:
    static int parseCode(const char *shaderPath, std::string &outCode, std::string const &token);

    static GLuint compileShader(std::string const &code, GLenum type);;
};


#endif //TFG_SHADER_H
