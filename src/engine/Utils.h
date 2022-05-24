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
#include "Event.h"
#include "Camera.h"
#include "Logger.h"


namespace tfg {

    struct WindowStruct {
        int width = 0;
        int height = 0;
        bool isFreeCamera = false;
        bool shouldClose = false;
        std::function<void(const Event &event)> eventCallback;
    };


    struct Transform {
        glm::vec3 position{0};
        glm::vec3 scale{1};
        glm::vec3 rotationAxis{1, 0, 0};
        float rotationDegress{0};
        glm::vec3 color{};

        [[nodiscard]] std::string to_string() const {
            return fmt::format("position {} {} {}\nscale {} {} {}\n",
                               position.x, position.y, position.z,
                               scale.x, scale.y, scale.z);
        }
    };

    struct Entity {
        int entityId{-1};
        bool isActive{true};
        glm::vec3 colorPick{1, 1, 1};

        [[nodiscard]] std::string to_string() const {
            return fmt::format("id {} {}\ncolorPick {} {} {}\n", entityId, isActive, colorPick.x, colorPick.y, colorPick.z);
        }
    };

    class Injector {
    public:
        Injector() = delete;

        static GLFWwindow *window;
        static WindowStruct *windowStruct;
        static Camera *camera;
        static int EntitySize;
    };

    struct Line {
        Line(glm::vec3 const &pointA, glm::vec3 const &pointB) : P0(pointA),
                                                                 D(glm::normalize(pointB - pointA)) {
            LOG_DEBUG("Line at {%0.1f} {%0.1f} {%0.1f} d = {%0.1f} {%0.1f} {%0.1f}",
                      P0.x, P0.y, P0.z,
                      D.x, D.y, D.z);
        }


        [[nodiscard]] bool distanceToPoint(glm::vec3 const &P) const {
            float t0 = glm::dot(D, P - P0) / glm::dot(D, D);
            return glm::distance(P, P0 + t0 * D);
        }

        glm::vec3 P0{0};
        glm::vec3 D{0};

    };

    struct Plane {
        Plane(Line const &line, glm::vec3 const &point) {
            // Build a perpedicular plane to line at point P
            setNormalVector(line.D);
            movePlaneToPoint(point);

            assert(planeContainsPoint(point));
        }

        [[nodiscard]] bool planeContainsPoint(glm::vec3 const &point) const {
            float dot = glm::dot(point, normal) + d;
            return glm::epsilonEqual(dot, 0.0f, 0.1f);
        }

        void setNormalVector(glm::vec3 const &dir) {
            normal = glm::normalize(dir);
        }

        [[nodiscard]] bool planeIsPerpendicularToLine(Line const &line) const {
            auto dot = glm::dot(line.D, normal);
            return glm::epsilonEqual(dot, 0.0f, 0.1f);
        }

        void movePlaneToPoint(glm::vec3 const &point) {
            // -(a*x + b*y + c*z) = d
            d = -glm::dot(point, normal);
            assert(planeContainsPoint(point));
        }

        glm::vec3 normal{0}; // normal vector
        float d{0};
    };


    void InitLogger();

    GLFWwindow *InitWindow(const char *title, int width = 1080, int height = 720);

    void ConfigureEvents(GLFWwindow *window);

    void DestroyWindow(GLFWwindow *window);

    glm::vec3 screenToWorld(int x, int y, glm::vec3 const &point);

    glm::vec3 screenToColor(int x, int y);

    int colorToId(glm::vec3 color);

    int getEntityId();

    glm::vec3 genPickColor(int n);

    void setFreeCamera(bool isEnabled);

    void glfw_error_callback(int error, const char *description);

    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *userParam);
}

typedef tfg::Injector Injector;


#endif //TFG_UTILS_H
