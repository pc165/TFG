#ifndef TFG_UTILS_H
#define TFG_UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>
#include <vector>
#include <string>
#include <functional>
#include "EventState.h"
#include "render/Camera.h"
#include "Logger.h"
#include "DataStructs.h"


namespace tfg {

    class Injector {
    public:
        Injector() = delete;

        static glm::vec3 clearColor;
        static GLFWwindow *window;
        static EventState *eventState;
        static Camera *camera;
        static bool shouldClose;
        static bool isFreeCamera;
        static int EntitySize;
    };


    void InitLogger();

    void InitWindow(const char *title, int width = 1080, int height = 720);

    void ConfigureEvents();

    void DestroyWindow();

    glm::vec3 screenToWorld(int x, int y);

    glm::vec3 screenToColor(int x, int y);

    void setClearColor(glm::vec4 const &color);

    int colorToId(glm::vec3 color);

    int getEntityId();

    glm::vec3 genPickColor(int n);

    void setFreeCamera(bool isEnabled);

    void glfw_error_callback(int error, const char *description);

    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *userParam);
}

typedef tfg::Injector Injector;


#endif //TFG_UTILS_H
