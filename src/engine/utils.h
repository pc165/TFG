#ifndef TFG_UTILS_H
#define TFG_UTILS_H

#include <string>
#include <vector>
#include "OpenGL.h"
#include "Camera.h"

struct EventStruct {
    int witdh = 0;
    int heigth = 0;
    bool shouldClose = false;
    Camera *camera = nullptr;
};


enum ShaderType {
    Vertex, Fragment
};

enum VectorType {
    Vec1 = 1,
    Vec2 = 2,
    Vec3 = 3,
    Vec4 = 4
};

enum Type {
    CHAR, INT, UINT, FLOAT, DOUBLE
};

struct ShaderLayouts {
    Type type;
    int location;
};

void glfw_error_callback(int error, const char *description);

void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *msg,
                                const void *userParam);


int parseCode(const char *shaderPath, std::string &outCode, ShaderType shaderType);

GLuint compileSource(std::string &vertexCode, std::string &fragmentCode);


#endif //TFG_UTILS_H
