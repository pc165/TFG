#ifndef TFG_SPHERE_H
#define TFG_SPHERE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


class Sphere : public Object {
public:
    explicit Sphere(Camera *camera, float radius = 1.0f, int sectorCount = 36, int stackCount = 18) : Object(*camera, false) {
        std::vector<float> position;
        std::vector<float> normals;
        std::vector<uint32_t> index;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord
        int k1, k2;

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack
            for (int j = 0; j <= sectorCount; ++j, ++k1, ++k2) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                position.push_back(x);
                position.push_back(y);
                position.push_back(z);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);


                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0) {
                    index.push_back(k1);
                    index.push_back(k2);
                    index.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1)) {
                    index.push_back(k1 + 1);
                    index.push_back(k2);
                    index.push_back(k2 + 1);
                }

            }
        }

        shader.loadSource("cube.glsl");
        vao.bind();
        ib.create(index.data(), index.size());

        // Position
        vb.create(position.data(), position.size() * sizeof(float));
        vao.addLayout(vb, 0, VectorType::Vec3, Type::FLOAT);

        vn.create(normals.data(), normals.size() * sizeof(float));
        vao.addLayout(vn, 1, VectorType::Vec3, Type::FLOAT);
    }

    void draw() override {
        vao.bind();
        shader.bind();

        shader.setMat4("view", glm::value_ptr(camera_.getViewMatrix()));
        shader.setMat4("projection", glm::value_ptr(camera_.getProjectionMatrix()));

        for (auto i = 0; i < size; i++) {
            auto tra = glm::translate(glm::mat4(1.0f), position_[i]);
            auto rot = glm::rotate(tra, glm::radians(degrees_[i]), rotationAxis_[i]);
            model_[i] = glm::scale(rot, scale_[i]);
        }
        for (auto i = 0; i < size; i++) {
            shader.setMat4("model", glm::value_ptr(model_[i]));
            shader.setVec3("colorIn", glm::value_ptr(color_[i]));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    void drawPickObject() override {
        vao.bind();
        shader.bind();

        shader.setMat4("view", glm::value_ptr(camera_.getViewMatrix()));
        shader.setMat4("projection", glm::value_ptr(camera_.getProjectionMatrix()));

        for (auto i = 0; i < size; i++) {
            auto tra = glm::translate(glm::mat4(1.0f), position_[i]);
            auto rot = glm::rotate(tra, glm::radians(degrees_[i]), rotationAxis_[i]);
            model_[i] = glm::scale(rot, scale_[i]);
        }

        for (auto i = 0; i < size; i++) {
            shader.setMat4("model", glm::value_ptr(model_[i]));
            shader.setVec3("colorIn", glm::value_ptr(pickColor_[i]));
            glDrawElements(GL_TRIANGLES, (GLsizei) ib.getCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    void createVertices() {

    }
};


#endif //TFG_SPHERE_H
