#ifndef TFG_CUBE_H
#define TFG_CUBE_H

#include "Object.hpp"

class Cube : public Object {
public:
    explicit Cube();

    void draw() const override;
};

#endif //TFG_CUBE_H
