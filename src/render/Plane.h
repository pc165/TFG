#ifndef TFG_PLANE_H
#define TFG_PLANE_H

#include "Object.h"

class Plane : public Object {
public:
    Plane() = default;

    void setPlane(tfg::Plane const &plane) {
        std::vector<glm::vec2> points = {
                {-1, 1},
                {1,  1},
                {1,  -1},
                {-1, -1}
        };

        std::vector<float> position;

        for (auto &i: points) {
            auto p = plane.getPointOnPlane(i);
            position.push_back(p.x);
            position.push_back(p.y);
            position.push_back(p.z);
        }

        std::vector<unsigned int> index = {
                0, 1, 2,
                0, 3, 2
        };

        std::vector<float> normals = {
                0, 0, 1,
                0, 0, 1,
                0, 0, 1,
                0, 0, 1,
        };

        setupVao(position, normals, index);
    };
};

#endif //TFG_PLANE_H
