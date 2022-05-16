#ifndef TFG_TOOLS_H
#define TFG_TOOLS_H

#include "OpenGL.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>

class Tools {
public:
    static glm::vec3 screenToWorld(int x, int y, Camera *camera) {
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};
        glReadPixels(x, int(win.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win.z);
        return glm::unProject(win, camera->getViewMatrix(), camera->getProjectionMatrix(), viewport);
    }


    static glm::vec3 screenToColor(int x, int y) {
        glm::vec3 color;
        glm::vec<4, int> viewport{0};
        glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
        glm::vec3 win{(float) x, viewport[3] - (float) y, 0};
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(x, int(win.y), 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(color));
        return color;
    }

};

#endif //TFG_TOOLS_H
