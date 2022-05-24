#ifndef TFG_SPHERE_H
#define TFG_SPHERE_H

#include "Object.hpp"


class Sphere : public Object {
public:
    explicit Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18) : Object() {
        std::vector<float> position;
        std::vector<float> normals;
        std::vector<unsigned int> index;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord
        int k1, k2;

        float sectorStep = 2 * glm::pi<float>() / sectorCount;
        float stackStep = glm::pi<float>() / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = glm::pi<float>() / 2 - i * stackStep;        // starting from pi/2 to -pi/2
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

        setupVao(position, normals, index);
    }
};


#endif //TFG_SPHERE_H
