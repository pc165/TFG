#ifndef TFG_CUBE_H
#define TFG_CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.hpp"
#include <glm/gtc/type_ptr.hpp>

class Cube : public Object {
public:
    explicit Cube() : Object() {
        std::vector<float> position = {
                1.0, -1.0, -1.0,
                1.0, -1.0, 1.0,
                -1.0, -1.0, 1.0,
                -1.0, -1.0, -1.0,
                1.0, 1.0, -1.0,
                1.0, 1.0, 1.0,
                -1.0, 1.0, 1.0,
                -1.0, 1.0, -1.0
        };
        std::vector<uint32_t> index = {
                4, 0, 3,
                4, 3, 7,
                2, 6, 7,
                2, 7, 3,
                1, 5, 2,
                5, 6, 2,
                0, 4, 1,
                4, 5, 1,
                4, 7, 5,
                7, 6, 5,
                0, 1, 2,
                0, 2, 3,
        };

        std::vector<float> normals = {
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,   // v0-v1-v2 (front)
                0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,   // v2-v3-v0
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,   // v0-v3-v4 (right)
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,   // v4-v5-v0
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,   // v0-v5-v6 (top)
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,   // v6-v1-v0
                -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,   // v1-v6-v7 (left)
                -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,   // v7-v2-v1
                0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,   // v7-v4-v3 (bottom)
                0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,   // v3-v2-v7
                0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,   // v4-v7-v6 (back)
                0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0    // v6-v5-v4
        };

        vao.bind();
        ib.create(index.data(), index.size());
        // Position
        vb.create(position.data(), position.size() * sizeof(float));
        vao.addLayout(vb, 0, VectorType::Vec3, Type::FLOAT);

        vn.create(normals.data(), normals.size() * sizeof(float));
        vao.addLayout(vn, 1, VectorType::Vec3, Type::FLOAT);
    }

    void draw() const override {
        Object::draw();
        // TODO
    }
};

#endif //TFG_CUBE_H
