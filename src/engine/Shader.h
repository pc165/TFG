#ifndef TFG_SHADER_H
#define TFG_SHADER_H

#include "IncludeLibraries.h"
#include "OpenGL.h"

class Shader {
private:
    uint32_t programId_, vertexShaderId_, fragmentShaderId_;
    std::string vertexShaderCode_;
    std::string fragmentShaderCode_;

public:
    ~Shader();

    Shader();

    uint32_t getProgramId() const { return programId_; }

    Shader(const char *vertexFilePath, const char *fragmentFilePath);

    explicit Shader(const char *shaderPath);

    void bind() const;

    void unBind() const;

    friend std::ostream &operator<<(std::ostream &os, const Shader &shader);

private:
    void compile();

    void link();

    static std::string readFile(const char *filePath);

    static void checkShader(uint32_t id);

    static void checkProgram(uint32_t id);
};


#endif //TFG_SHADER_H
