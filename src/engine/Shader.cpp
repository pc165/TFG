#include "Shader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

int parseCode(const char *shaderPath, std::string &outCode, ShaderType shaderType) {
    std::string token;
    switch (shaderType) {
        case Vertex:
            token = "vertex";
            break;
        case Fragment:
            token = "fragment";
            break;
    }

    std::ifstream source(shaderPath, std::ios::in);
    std::stringstream ss;
    std::string line;
    while (getline(source, line)) {
        if (line.find("#begin " + token) != std::string::npos) {
            LOG_INFO("Found begin token {}", line);
            while (getline(source, line) &&
                   line.find("#end " + token) == std::string::npos) {
                ss << line << "\n";
            }
            LOG_INFO("Found end token {}", line);
            outCode = ss.str();
            return 0;
        }
    }
    LOG_WARN("Syntax Error");
    return -1;
}

GLuint compileSource(std::string &vertexCode, std::string &fragmentCode) {
    // Create the shaders
    GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

    GLint result = GL_FALSE;
    int length;

    // Compile Vertex Shader
    char const *vertexCodePtr = vertexCode.c_str();
    glShaderSource(vertexId, 1, &vertexCodePtr, nullptr);
    glCompileShader(vertexId);

    // Check Vertex Shader
    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexId, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        std::vector<char> msg(length + 1);
        glGetShaderInfoLog(vertexId, length, nullptr, &msg[0]);
        LOG_ERROR("{}", &msg[0]);
    }

    // Compile Fragment Shader
    char const *FragmentSourcePointer = fragmentCode.c_str();
    glShaderSource(fragmentId, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(fragmentId);

    // Check Fragment Shader
    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentId, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        std::vector<char> msg(length + 1);
        glGetShaderInfoLog(fragmentId, length, nullptr, &msg[0]);
        LOG_ERROR("{}", &msg[0]);
    }

    // Link the program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    glLinkProgram(programId);

    // Check the program
    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        std::vector<char> msg(length + 1);
        glGetProgramInfoLog(programId, length, nullptr, &msg[0]);
        LOG_ERROR("{}", &msg[0]);
    }

    glDetachShader(programId, vertexId);
    glDetachShader(programId, fragmentId);

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    return programId;
}

void Shader::loadSource(const char *path) {
    if (parseCode(path, vertexCode, ShaderType::Vertex))
        LOG_ERROR("Cannot load code");
    if (parseCode(path, fragmentCode, ShaderType::Fragment))
        LOG_ERROR("Cannot load code");
    programId = compileSource(vertexCode, fragmentCode);
    LOG_INFO("Created program {}", programId);
}

void Shader::setUniformMatrix4fv(const char *name, int32_t count, bool transpose, void *ptrData) const {
    bind();
    auto location = glGetUniformLocation(programId, name);
    glUniformMatrix4fv(location, count, transpose ? GL_TRUE : GL_FALSE, (GLfloat *) ptrData);
}

void Shader::bind() const {
    glUseProgram(programId);
}

void Shader::unBind() {
    glUseProgram(0);
}
