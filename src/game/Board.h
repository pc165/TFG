#ifndef TFG_BOARD_H
#define TFG_BOARD_H

#include "Sphere.h"
#include "Cube.h"
#include <unordered_map>

class Board {
public:
    explicit Board() : cube_(), sphere_() {}

    void draw() const {
        cube_.draw();
        sphere_.draw();
    }

    void drawPickObject() const {
        cube_.drawPickObject();
        sphere_.drawPickObject();
    }

    void addTile(glm::vec3 pos, int number, float scale = 0.5f) {
        assert(number >= -1 && number <= 9);

        /***
         * - - - - -
         * - 0 - 0 -
         * - 0 - 0 -
         * - 0 - 0 -
         * - - - - -
         * */
        const glm::vec3 cubeScale(0.5 * scale, 0.8 * scale, 0.3 * scale);
        const glm::vec3 sphereScale(0.1 * scale, 0.1 * scale, 0.1 * scale);
        const glm::vec3 sphereColor(0, 1, 0);

        // set initial position to pos
        glm::vec3 spherePos{pos};

        // bring the sphere to the front
        spherePos.z += cubeScale.z;
        std::vector<int> sphereId{};

        // use number of cubes for spheres id and cube id
        int objectId = cube_.getSize();

        if (object2Number_.find(objectId) != object2Number_.cend()) {
            LOG_ERROR("Duplicate objectId: {} Number: {}", objectId, number);
            return;
        }
        // save number
        object2Number_[objectId] = number;

        cube_.add(objectId, pos, cubeScale, {1, 0, 0});

        /***
         *      Braille numbers
         *  1     2     3     4     5
         * 1 0   1 0   1 1   1 1   1 0
         * 0 0   1 0   0 0   0 1   0 1
         * 0 0   0 0   0 0   0 0   0 0
         *
         *  6     7     8     9     0
         * 1 1   1 1   1 0   0 1   0 1
         * 1 0   1 1   1 1   1 0   1 1
         * 0 0   0 0   0 0   0 0   0 0
         * */


        /***
         * - - - - -
         * - 0 - 0 -
         * - 0 - 0 - <<<<<----
         * - 0 - 0 -
         * - - - - -
         *
         * 1     2     3     4     5
         * 0 0   1 0   0 0   0 1   0 1
         *
         * 6     7     8     9     0
         * 1 0   1 1   1 1   1 0   1 1
         *
         **/

        // left
        if (number >= 6 ||
            number == 0 ||
            number == 2 ||
            number == -1) {
            spherePos.x = -cubeScale.x / 3 + pos.x;
            auto id = sphere_.add(objectId, spherePos, sphereScale, sphereColor);
            sphereId.push_back(id);
        }

        // right
        if (number == 4 || number == 5 ||
            number == 7 || number == 8 ||
            number == 0 ||
            number == -1) {
            spherePos.x = +cubeScale.x / 3 + pos.x;
            auto id = sphere_.add(objectId, spherePos, sphereScale, sphereColor);
            sphereId.push_back(id);
        }

        /***
         * - - - - -
         * - 0 - 0 - <<<<<----
         * - 0 - 0 -
         * - 0 - 0 -
         * - - - - -
         *
         * 1     2     3     4     5
         * 1 0   1 0   1 1   1 1   1 0
         *
         * 6     7     8     9     0
         * 1 1   1 1   1 0   0 1   0 1
         *
         **/

        spherePos.y = +cubeScale.y / 2 + pos.y;

        // left
        if (number >= 1 && number <= 8) {
            spherePos.x = -cubeScale.x / 3 + pos.x;
            auto id = sphere_.add(objectId, spherePos, sphereScale, sphereColor);
            sphereId.push_back(id);
        }

        // right
        if (number == 3 || number == 4 ||
            number == 6 || number == 7 ||
            number == 9 ||
            number == 0 ||
            number == -1) {
            spherePos.x = +cubeScale.x / 3 + pos.x;
            auto id = sphere_.add(objectId, spherePos, sphereScale, sphereColor);
            sphereId.push_back(id);
        }


        /***
         * - - - - -
         * - 0 - 0 -
         * - 0 - 0 -
         * - 0 - 0 - <<<<<----
         * - - - - -
         *
         * 1     2     3     4     5
         * 0 0   0 0   0 0   0 0   0 0
         *
         * 6     7     8     9     0
         * 0 0   0 0   0 0   0 0   0 0
         *
         **/

        spherePos.y = -cubeScale.y / 2 + pos.y;
        if (number == -1) {
            // left
            spherePos.x = -cubeScale.x / 3 + pos.x;
            auto id = sphere_.add(objectId, spherePos, sphereScale, sphereColor);
            sphereId.push_back(id);

            // right
            spherePos.x = +cubeScale.x / 3 + pos.x;
            id = sphere_.add(objectId, spherePos, sphereScale, sphereColor);
            sphereId.push_back(id);
        }

        sphereIds_[objectId] = sphereId;
    }


    void moveTile(int objectId, glm::vec3 pos) {
        assert(objectId < cube_.getSize());

    }

    int getTileValue(int objectId) const {
        assert(objectId < cube_.getSize());
        auto v = object2Number_.find(objectId);
        if (v == object2Number_.cend()) {
            return -1;
        }
        return v->second;
    }

private:
    std::unordered_map<int, int> object2Number_{};
    std::unordered_map<int, std::vector<int>> sphereIds_{};
    Cube cube_;
    Sphere sphere_;
};

#endif //TFG_BOARD_H
