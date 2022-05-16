#ifndef TFG_BOARD_H
#define TFG_BOARD_H

#include "Sphere.h"
#include "Cube.h"

class Board {
public:
    explicit Board(Camera *camera) : cube_(camera), sphere_(camera) {
    }

    void draw() {
        cube_.draw();
        sphere_.draw();
    }

    void drawPickObject() {
        cube_.drawPickObject();
        sphere_.drawPickObject();
    }

    void addTile(glm::vec3 pos, int number) {
        cube_.add(pos);
    }

private:
    Cube cube_;
    Sphere sphere_;
};

#endif //TFG_BOARD_H
