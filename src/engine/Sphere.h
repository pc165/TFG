#ifndef TFG_SPHERE_H
#define TFG_SPHERE_H

#include <cmath>
#include "Object.hpp"


class Sphere : public Object {
public:
    explicit Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
};


#endif //TFG_SPHERE_H
