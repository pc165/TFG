#ifndef TFG_TILE_H
#define TFG_TILE_H

#include "Sphere.h"
#include "Cube.h"

struct TileData : Entity {
    int numericalValue{0};
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
};

class Tile {
public:
    explicit Tile() : number2Braille_(), cube_(), sphere_() {
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
                {1, 2, 3}, // 0
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

    void addTile(glm::vec3 const &pos, int numericalValue, float scale = 0.5f) {
        TileData tile{};

        tile.numericalValue = numericalValue;
        tile.entityId = Tools::getEntityId();
        tile.colorPick = Tools::genPickColor(tile.entityId);

        // Setup cube
        tile.cube.position = pos;
        tile.cube.scale = glm::vec3{0.5 * scale, 0.8 * scale, 0.3 * scale};
        tile.cube.color = {1, 0, 0};


        // sphere position offsets
        auto xOffset = tile.cube.scale.x / 3;
        auto yOffset = tile.cube.scale.y / 2;

        // top left
        tile.sphere[0].position = glm::vec3{-xOffset, yOffset, 0};

        // top right
        tile.sphere[1].position = glm::vec3{xOffset, yOffset, 0};

        // middle left
        tile.sphere[2].position = glm::vec3{-xOffset, 0, 0};

        // middle right
        tile.sphere[3].position = glm::vec3{xOffset, 0, 0};

        // bottom left
        tile.sphere[4].position = glm::vec3{-xOffset, -yOffset, 0};

        // bottom right
        tile.sphere[5].position = glm::vec3{xOffset, -yOffset, 0};


        // Setup sphere
        for (auto &i: tile.sphere) {
            // translate spheres
            i.position += pos;
            i.position.z += tile.cube.scale.z;

            i.scale = glm::vec3{0.1f * scale};
            i.color = {0, 1, 0};
        }

        tile.log();
        tileData_.emplace_back(tile);
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

private:
    std::vector<std::vector<size_t>> number2Braille_;
    std::vector<TileData> tileData_;
    Cube cube_;
    Sphere sphere_;
};

#endif //TFG_TILE_H
