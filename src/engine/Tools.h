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

    static glm::vec3 screenToWorld(int x, int y) {
        assert(camera != nullptr);
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

        // convert to open GL coordinates
        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};
        glReadPixels((int) win.x, (int) win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);

        glm::vec3 pos = glm::unProject(win, Tools::camera->getViewMatrix(), Tools::camera->getProjectionMatrix(), viewport);
        return pos;
    }


    static glm::vec3 screenToColor(int x, int y) {
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};

        if (windowStruct->isFreeCamera) {
            win.x = viewport[2] / 2.0;
            win.y = viewport[3] / 2.0;
        }

        if (win.x < viewport[0] || win.x > viewport[2] ||
            win.y < viewport[1] || win.y > viewport[3]) {
            glm::vec3 clearColor;
            LOG_TRACE("Mouse outside viewport");
            glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clearColor));
            return clearColor;
        }

        glm::vec3 color;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels((int) win.x, (int) win.y, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(color));

        return color;
    }

    static int colorToId(glm::vec3 color) {
        glm::vec3 clearColor;
        glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clearColor));

        if (color == clearColor)
            return -1;

        int id = std::round(color.r * 10) +
                 std::round(color.g * 100) +
                 std::round(color.b * 1000);

        return id;
    };

    static glm::vec3 genPickColor(int n) {
        float r = int((n % 10)) / 10.f;
        float g = int((n % 100) / 10) / 10.f;
        float b = int((n % 1000) / 100) / 100.f;
        glm::vec3 newColor(r, g, b);

        LOG_DEBUG("Pick color {}", glm::to_string(newColor).c_str());
        return newColor;
    }

    static void setFreeCamera(bool isEnabled) {
        if (isEnabled) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        glfwSetCursorPos(window, windowStruct->width / 2.0, windowStruct->height / 2.0);
        camera->setFirstMove();
        camera->setFreeCamera(isEnabled);
        windowStruct->isFreeCamera = isEnabled;
    }
};

#endif //TFG_TOOLS_H
