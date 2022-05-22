#ifndef TFG_INJECTOR_H
#define TFG_INJECTOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
#include "Window.h"
#include "Entity.h"

class Injector {
public:
    static GLFWwindow *window;
    static WindowStruct *windowStruct;
    static Camera *camera;
    static int EntitySize;

    Injector() = delete;

    static glm::vec3 screenToWorld(int x, int y, glm::vec3 const &point);

    static glm::vec3 screenToColor(int x, int y);

    static int colorToId(glm::vec3 color);

    static int getEntityId() {
        return Injector::EntitySize++;
    }

    static glm::vec3 genPickColor(int n);

    static void setFreeCamera(bool isEnabled);
};

#endif //TFG_INJECTOR_H
