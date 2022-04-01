#ifndef TFG_SHADER_H
#define TFG_SHADER_H

#include <string>
#include "OpenGL.h"
#include "Logger.h"

enum ShaderType {
    Vertex, Fragment
};

int parseCode(const char *shaderPath, std::string &outCode, ShaderType shaderType);

GLuint compileSource(std::string &vertexCode, std::string &fragmentCode);

class Shader {
private:
    std::string vertexCode{};
    std::string fragmentCode{};
    GLuint programId{};
public:
    Shader() = default;

    [[nodiscard]] GLuint getProgramId() const { return programId; }

    ~Shader() { glDeleteProgram(programId); }

    void loadSource(const char *path);

    void setUniformMatrix4fv(const char *name, int32_t count, bool transpose, void *ptrData) const;

    void bind() const;;

    static void unBind();

    void parseSource() {

    }
};


#endif //TFG_SHADER_H
