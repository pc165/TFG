#ifndef TFG_SHADER_H
#define TFG_SHADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


#include <GL/glew.h>
#include <sstream>

class Shader {
private:
    uint32_t programId_, vertexShaderId_, fragmentShaderId_;
public:
    uint32_t getProgramId() const { return programId_; }

private:
    std::string vertexShaderCode_;
    std::string fragmentShaderCode_;
public:
    ~Shader();

    Shader();

    Shader(const char *vertexFilePath, const char *fragmentFilePath);

    Shader(const char *shaderPath);

    void bind() const;

    __attribute__((unused)) static void unBind();

private:
    void compile();

    void link();

    static std::string readFile(const char *filePath);

    static void checkShader(uint32_t id, GLenum pname);

    static void checkProgram(uint32_t id, GLenum pname);
};


#endif //TFG_SHADER_H
