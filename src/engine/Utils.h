#ifndef TFG_UTILS_H
#define TFG_UTILS_H

#include "OpenGL.h"
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
    struct Globals {
        glm::vec3 clearColor{0};
        Camera *camera{nullptr};
        bool isFreeCamera{false};
        bool shouldClose{false};
        bool drawCubeNormals{false};
        bool drawSphereNormals{false};
        bool drawPlaneNormals{false};
        Light light;
    };

    class Injector {
    public:
        Injector() = delete;

        static GLFWwindow *window;
        static EventState *eventState;
        static int EntitySize;
        static struct Globals globals;
    };


    void InitLogger();

    void InitWindow(const char *title, int width = 1080, int height = 720, bool fullscreen = true);

    void ConfigureEvents();

    void DestroyWindow();

    glm::vec3 screenToWorld(int x, int y);

    glm::vec3 screenToColor(int x, int y);

    void setClearColor(glm::vec4 const &color);

    int colorToId(glm::vec3 color);

    int getEntityId();

    glm::vec3 genPickColor(int n);

    void setFreeCamera(bool isEnabled);

    unsigned int colorToTexture(glm::vec3 color, const int size);

    void glfw_error_callback(int error, const char *description);

    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *userParam);
}

typedef tfg::Injector Injector;
static auto &GlobalOptions = Injector::globals;

#endif //TFG_UTILS_H
