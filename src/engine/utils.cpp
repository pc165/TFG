#include <iostream>
#include <csignal>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "utils.h"
#include "Logger.h"

void glfw_error_callback(int error, const char *description) {
    LOG_ERROR("Glfw Error {}: {}", error, description);
}

void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *msg,
                                const void *userParam) {
    std::string _source;
    std::string _type;
    std::string _severity;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;
        default:
            _source = "UNKNOWN";
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;
        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;
        default:
            _type = "UNKNOWN";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;
        default:
            _severity = "UNKNOWN";
            break;
    }
    LOG_ERROR("{} : {} of {} severity, raised from {}: {}",
              id, _type.c_str(), _severity.c_str(), _source.c_str(), msg);
    raise(SIGTRAP);
    exit(1);
}

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