#include "Shader.h"

GLuint Shader::getProgramId() const { return programId; }

Shader::~Shader() { glDeleteProgram(programId); }

void Shader::   loadSource(const char *path) {
    assert(programId == 0);
    std::string vertexCode{};
    std::string fragmentCode{};
    std::string geometricCode{};

    LOG_INFO("Loading {}", path);

    if (parseCode(path, vertexCode, "vertex"))
        LOG_ERROR("Cannot parse vertex shader");

    if (parseCode(path, fragmentCode, "fragment"))
        LOG_ERROR("Cannot parse fragment shader");

    if (parseCode(path, geometricCode, "geometric"))
        LOG_ERROR("Cannot parse geometric shader");


    // Create the shaders
    GLuint vertexId = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentId = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
    GLuint geometricId = compileShader(geometricCode, GL_GEOMETRY_SHADER);

    GLboolean result = GL_FALSE;
    int length;

    // Link the program
    programId = glCreateProgram();
    glAttachShader(programId, vertexId);
    glAttachShader(programId, fragmentId);
    if (geometricId)
        glAttachShader(programId, geometricId);

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
    if (geometricId)
        glDetachShader(programId, geometricId);

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    LOG_INFO("Created program {}", programId);
}

int Shader::parseCode(const char *shaderPath, std::string &outCode, std::string const &token) {
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
    LOG_WARN("token {} not found", token);
    return -1;
}