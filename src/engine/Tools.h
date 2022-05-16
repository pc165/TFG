#ifndef TFG_TOOLS_H
#define TFG_TOOLS_H

#include "OpenGL.h"
#include "Camera.h"
#include "Window.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Tools {
public:
    static GLFWwindow *window;
    static WindowStruct *windowStruct;
    static Camera *camera;

    static glm::vec3 screenToWorld(int x, int y);

    static glm::vec3 screenToColor(int x, int y);

    static int colorToId(glm::vec3 color);;

    static glm::vec3 genPickColor(int n);

    static void setFreeCamera(bool isEnabled);
};

#endif //TFG_TOOLS_H
