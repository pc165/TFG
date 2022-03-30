#ifndef TFG_SHADER_H
#define TFG_SHADER_H

#include <string>
#include "Utils.h"
#include "Logger.h"
#include "strings.h"
#include "OpenGL.h"

class Shader {
private:
    std::string vertexCode{};
    std::string fragmentCode{};
    GLuint programId{};
public:
    Shader() = default;

    [[nodiscard]] GLuint getProgramId() const { return programId; }

    ~Shader() { glDeleteProgram(programId); }

    void loadSource(const char *path) {
        if (parseCode(path, vertexCode, ShaderType::Vertex))
            LOG_ERROR("Cannot load code");
        if (parseCode(path, fragmentCode, ShaderType::Fragment))
            LOG_ERROR("Cannot load code");
        programId = compileSource(vertexCode, fragmentCode);
        LOG_INFO("Created program {}", programId);
    }

    void setUniformMatrix4fv(const char *name, int32_t count, bool transpose, void *ptrData) const {
        bind();
        auto location = glGetUniformLocation(programId, name);
        glUniformMatrix4fv(location, count, transpose ? GL_TRUE : GL_FALSE, (GLfloat *) ptrData);
    }

    void bind() const {
        glUseProgram(programId);
    };

    static void unBind() {
        glUseProgram(0);
    }

    void parseSource() {

    }
};


#endif //TFG_SHADER_H
