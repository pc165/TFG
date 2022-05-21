#ifndef TFG_BOARD_H
#define TFG_BOARD_H

#include "Sphere.h"
#include "Cube.h"

struct Tile : Entity {
    int numericalValue{0};
    int row = -1, col = -1;
    Transform cube{};
    std::vector<Transform> sphere{6};

    void log() {
        std::string a;
        int j = 0;
        for (auto &i: sphere) {
            a += fmt::format("Sphere {}\n{}\n", j++, i.to_string());
        }
        LOG_DEBUG("\n{}\nCube\n{}\n{}\n", this->to_string(), cube.to_string(), a);
    }


    void updatePosition(glm::vec3 const &pos) {
        cube.position = pos;

        // sphere position offsets
        auto xOffset = cube.scale.x / 3;
        auto yOffset = cube.scale.y / 2;
        auto zOffset = cube.scale.z;

        // top left
        sphere[0].position = glm::vec3{-xOffset, yOffset, zOffset} + pos;

        // top right
        sphere[1].position = glm::vec3{xOffset, yOffset, zOffset} + pos;

        // middle left
        sphere[2].position = glm::vec3{-xOffset, 0, zOffset} + pos;

        // middle right
        sphere[3].position = glm::vec3{xOffset, 0, zOffset} + pos;

        // bottom left
        sphere[4].position = glm::vec3{-xOffset, -yOffset, zOffset} + pos;

        // bottom right
        sphere[5].position = glm::vec3{xOffset, -yOffset, zOffset} + pos;
    }

};

class Board {
public:
    explicit Board() : number2Braille_(), cube_(), sphere_() {
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
        number2Braille_ = {
//                {1, 2, 3}, // 0
                {}, // 0
                {0}, // 1
                {0, 2}, // 2
                {0, 1}, // 3
                {0, 1, 3}, // 4
                {0, 3}, // 5
                {0, 1, 2}, // 6
                {0, 1, 2, 3}, // 7
                {0, 2, 3}, // 8
                {1, 2}, // 9
                {0, 1, 2, 3, 4, 5} // 10 draw everything
        };
    }

    auto &addTile(glm::vec3 const &pos, int numericalValue, int col, int row, float scale = 0.5f) {
        Tile tile{};
        tile.row = row;
        tile.col = col;
        tile.numericalValue = numericalValue;
        tile.entityId = Tools::getEntityId();
        tile.colorPick = Tools::genPickColor(tile.entityId);

        // Setup cube
        tile.cube.scale = glm::vec3{0.5 * scale, 0.8 * scale, 0.3 * scale};
        tile.cube.color = {1, 0, 0};
        tile.updatePosition(pos);

        // Setup sphere
        for (auto &i: tile.sphere) {
            // translate spheres
            i.scale = glm::vec3{0.1f * scale};
            i.color = {0, 1, 0};
        }

        tile.log();
        tileData_.emplace_back(tile);
        return tileData_[tileData_.size() - 1];
    }

    void drawBoard(bool isPicking = false) {
        for (auto &tile: tileData_) {
            auto cubeColor = isPicking ? tile.colorPick : tile.cube.color;
            cube_.draw(tile.cube, cubeColor);
            auto sphereColor = isPicking ? tile.colorPick : tile.sphere[0].color;
            for (auto i: number2Braille_[tile.numericalValue]) {
                sphere_.draw(tile.sphere[i], sphereColor);
            }
        }
    }

    void moveTile(int entityId, glm::vec3 pos) {
        pos.z = 0;
        for (auto &i: tileData_) {
            if (i.entityId == entityId) {
                i.updatePosition(pos);
                return;
            }
        }
    }

    Tile *getTile(int entityId) {
        for (auto &i: tileData_) {
            if (i.entityId == entityId)
                return &i;
        }
        return nullptr;
    }

private:
    std::vector<std::vector<size_t>> number2Braille_;
    std::vector<Tile> tileData_;
    Cube cube_;
    Sphere sphere_;
};

#endif //TFG_BOARD_H
