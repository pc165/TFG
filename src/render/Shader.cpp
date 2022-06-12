#include "Shader.h"

GLuint Shader::getProgramId() const { return programId; }

Shader::~Shader() { glDeleteProgram(programId); }

void Shader::loadSource(const char *path) {
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


    // get shader uniforms
    int numUniforms;
    int maxCharLength;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &numUniforms);
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxCharLength);
    if (numUniforms > 0 && maxCharLength > 0) {
        LOG_INFO("Found {} uniforms", numUniforms);
        char *buffer = (char *) new char[maxCharLength];
        for (int i = 0; i < numUniforms; i++) {
            int size;
            GLenum dataType;
            glGetActiveUniform(programId, i, maxCharLength, &length, &size, &dataType, buffer);
            location[buffer] = glGetUniformLocation(programId, buffer);
            LOG_INFO("{} {}", buffer, location[buffer]);
        }
        delete[]buffer;
    }


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

int Shader::getLocation(const std::string &name) const {
    auto a = location.find(name);
    assert(a != location.end());
    return a->second;
}

void Shader::setBoolean(const std::string &name, int ptrData) const {
    glUniform1i(getLocation(name), ptrData);
}

void Shader::setMat4(const std::string &name, const float *ptrData) const {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, ptrData);
}

void Shader::setMat3(const std::string &name, const float *ptrData) const {
    glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, ptrData);
}

void Shader::setVec4(const std::string &name, const float *ptrData) const {
    glUniform4fv(getLocation(name), 1, ptrData);
}

void Shader::setVec3(const std::string &name, const float *ptrData) const {
    glUniform3fv(getLocation(name), 1, ptrData);
}

void Shader::setVec2(const std::string &name, const float *ptrData) const {
    glUniform2fv(getLocation(name), 1, ptrData);
}

void Shader::setFloat(const std::string &name, float ptrData) const {
    glUniform1f(getLocation(name), ptrData);
}

void Shader::setInt(const std::string &name, int ptrData) const {
    glUniform1i(getLocation(name), ptrData);
}

void Shader::setLight(const tfg::Light &light) const {
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

void Shader::setMaterial(const tfg::Material &material) const {
    setFloat("uMaterial.shininess", material.shininess);
    setInt("uMaterial.diffuse", material.diffuse);
    setInt("uMaterial.specular", material.specular);
}

void Shader::bind() const {
    glUseProgram(programId);
}

void Shader::unBind() {
    glUseProgram(0);
}

GLuint Shader::compileShader(const std::string &code, GLenum type) {
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
}
