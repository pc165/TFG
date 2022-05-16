#ifndef TFG_TOOLS_H
#define TFG_TOOLS_H

#include "OpenGL.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Tools {
public:
    static glm::vec3 screenToWorld(int x, int y, Camera *camera) {
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

        // convert to open GL coordinates
        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};
        glReadPixels((int) win.x, (int) win.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);

        glm::vec3 pos = glm::unProject(win, camera->getViewMatrix(), camera->getProjectionMatrix(), viewport);

        LOG_TRACE("gl unProject {}", glm::to_string(pos).c_str());

        return pos;
    }


    static glm::vec3 screenToColor(int x, int y) {
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));

        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};

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

        LOG_TRACE("Read color {}", glm::to_string(color).c_str());
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

        LOG_TRACE("object id {}", id);
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
};

#endif //TFG_TOOLS_H
