#ifndef TFG_TOOLS_H
#define TFG_TOOLS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
#include "Window.h"
#include "Entity.h"

class Tools {
public:
    static GLFWwindow *window;
    static WindowStruct *windowStruct;
    static Camera *camera;
    static int EntitySize;

    Tools() = delete;

    static glm::vec3 screenToWorld(int x, int y);

    static glm::vec3 screenToColor(int x, int y);

    static int colorToId(glm::vec3 color);

    static int getEntityId() {
        return Tools::EntitySize++;
    }

    static glm::vec3 genPickColor(int n);

    static void setFreeCamera(bool isEnabled);
};

#endif //TFG_TOOLS_H
