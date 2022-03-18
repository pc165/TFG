#include "Shader.h"

Shader::Shader() {
    fragmentShaderCode_ =
            "#version 330 core\n"
            "out vec4 color;\n"
            "void main(){\n"
            "color = vec4(1.0, 0.0, 0.0, 0.0);\n"
            "}\n";
    vertexShaderCode_ =
            "#version 330 core\n"
            "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
            "void main(){\n"
            "gl_Position.xyz = vertexPosition_modelspace;\n"
            "gl_Position.w = 1.0;\n"
            "}\n";
    compile();
    link();
}

Shader::Shader(const char *vertexFilePath, const char *fragmentFilePath) {
    fragmentShaderCode_ = readFile(fragmentFilePath);
    vertexShaderCode_ = readFile(vertexFilePath);
    compile();
    link();
}


Shader::Shader(const char *shaderPath) {
    const std::string vertexToken = "#VERTEX\n", fragmentToken = "#FRAGMENT\n";

    std::string source = readFile(shaderPath);
    size_t idxVertex = source.find(vertexToken, 0);
    size_t idxFragment = source.find(fragmentToken, 0);

    if (idxFragment == std::string::npos ||
        idxVertex == std::string::npos ||
        idxVertex >= idxFragment) {
        fprintf(stderr, "Syntax error in %s\n", shaderPath);
    }

    size_t fragmentSize = source.size() - idxVertex;
    size_t vertexSize = idxFragment - fragmentToken.size();

    vertexShaderCode_ = source.substr(idxVertex + vertexToken.size(), vertexSize);
    fragmentShaderCode_ = source.substr(idxFragment + fragmentToken.size(), fragmentSize);

    compile();
    link();
}

void Shader::compile() {
    // Create the shaders
    vertexShaderId_ = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderId_ = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile Vertex Shader
    char const *vertexSourcePointer = vertexShaderCode_.c_str();
    glShaderSource(vertexShaderId_, 1, &vertexSourcePointer, nullptr);
    glCompileShader(vertexShaderId_);
    checkShader(vertexShaderId_, GL_COMPILE_STATUS);

    // Compile Fragment Shader
    char const *FragmentSourcePointer = fragmentShaderCode_.c_str();
    glShaderSource(fragmentShaderId_, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(fragmentShaderId_);
    checkShader(fragmentShaderId_, GL_COMPILE_STATUS);
}


void Shader::link() {
    // Link the program
    printf("Linking program\n");
    programId_ = glCreateProgram();
    glAttachShader(programId_, vertexShaderId_);
    glAttachShader(programId_, fragmentShaderId_);
    glLinkProgram(programId_);

    checkProgram(programId_, GL_LINK_STATUS);

    glDetachShader(programId_, vertexShaderId_);
    glDetachShader(programId_, fragmentShaderId_);
}

void Shader::checkShader(uint32_t id, GLenum pname) {
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(id, pname, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(id, infoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
}

void Shader::checkProgram(uint32_t id, GLenum pname) {
    // Check Vertex Shader
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetProgramiv(id, pname, &result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(id, infoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }
}


std::string Shader::readFile(const char *filePath) {
    std::string result;
    std::ifstream in(filePath, std::ios::in);
    if (in) {
        in.seekg(0, std::ios::end);
        auto size = in.tellg();
        if (size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        }
    }
    return result;
}

void Shader::bind() const {
    glUseProgram(programId_);
}

void Shader::unBind() {
    glUseProgram(0);
}

Shader::~Shader() {
    glDeleteProgram(programId_);
    glDeleteShader(vertexShaderId_);
    glDeleteShader(fragmentShaderId_);
}
