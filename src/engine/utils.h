#ifndef TFG_UTILS_H
#define TFG_UTILS_H

#include "IncludeLibraries.h"

void glfw_error_callback(int error, const char *description);

void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *msg,
                                const void *userParam);

#endif //TFG_UTILS_H
