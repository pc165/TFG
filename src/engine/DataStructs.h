#ifndef TFG_DATASTRUCTS_H
#define TFG_DATASTRUCTS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include "Logger.h"

namespace tfg {
    struct WindowStruct {
        int width = 0;
        int height = 0;
        bool isFreeCamera = false;
        bool shouldClose = false;
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

    struct Line {
        Line(glm::vec3 const &pointA, glm::vec3 const &pointB) : origin(pointA),
                                                                 direction(glm::normalize(pointB - pointA)) {
            LOG_DEBUG("Line o = {} d = {}", glm::to_string(origin), glm::to_string(direction));
        }


        [[nodiscard]] bool distanceToPoint(glm::vec3 const &P) const {
            float t0 = glm::dot(direction, P - origin) / glm::dot(direction, direction);
            return glm::distance(P, origin + t0 * direction);
        }

        glm::vec3 origin{0};
        glm::vec3 direction{0};

    };

    struct Plane {
        void log() const {
            LOG_INFO("Normal {}, D {}", glm::to_string(normal), d);
        }

        void fromPoints(glm::vec3 const &pointA, glm::vec3 const &pointB, glm::vec3 const &pointC) {
            auto n = glm::cross(pointB - pointA, pointC - pointA);
            assert(n != glm::vec3{0});
            setNormalVector(n);
            movePlaneToPoint(pointB);

            assert(containsPoint(pointA));
            assert(containsPoint(pointB));
            assert(containsPoint(pointC));
        }


        void fromNormalPoint(Line const &line, glm::vec3 const &point) {
            // Build a perpedicular plane to line at point P
            setNormalVector(line.direction);
            movePlaneToPoint(point);

            assert(containsPoint(point));
        }

        [[nodiscard]] bool containsPoint(glm::vec3 const &point) const {
            float dot = glm::dot(point, normal) + d;
            return glm::epsilonEqual(dot, 0.0f, 0.1f);
        }

        void setNormalVector(glm::vec3 const &dir) {
            normal = glm::normalize(dir);
        }

        [[nodiscard]] bool planeIsPerpendicularToLine(Line const &line) const {
            auto dot = glm::dot(line.direction, normal);
            return glm::epsilonEqual(dot, 0.0f, 0.1f);
        }

        void movePlaneToPoint(glm::vec3 const &point) {
            // -(a*x + b*y + c*z) = d
            d = -glm::dot(point, normal);
            assert(containsPoint(point));
        }

        glm::vec3 getPointOnPlane(glm::vec2 const &point) const {
            /***
             * p0 = Ax + By + Cz + D = 0
             *  x = -(By + Cz + D)/A
             *  y = -(Ax + Cz + D)/B
             *  z = -(Ax + By + D)/C
             * */
            auto z = -(point.x * normal.x + point.y * normal.y + d) / normal.z;
            glm::vec3 p{point, z};
            assert(containsPoint(p));
            return p;
        }

        glm::vec3 normal{0}; // normal vector
        float d{0}; // offset
        glm::vec3 p0{0}, p1{0}, p2{0}, p3{0};
    };

}
#endif //TFG_DATASTRUCTS_H
