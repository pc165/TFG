#ifndef TFG_BOARD_H
#define TFG_BOARD_H

#include "Sphere.h"
#include "Cube.h"
#include "Plane.h"

struct Tile : tfg::Entity {
    int numericalValue{0};
    int row = -1, col = -1;
    bool isSelected{false};
    tfg::Transform cube{};
    std::vector<tfg::Transform> sphere{6};

    void log() {
        std::string a;
        int j = 0;
        for (auto &i: sphere) {
            a += fmt::format("Sphere {}\n{}\n", j++, i.to_string());
        }
        LOG_DEBUG("\n{}\nCube\n{}\n{}\n", this->to_string(), cube.to_string(), "");
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
    explicit Board() : number2Braille_(), cubeRender_(), planeRender_(), sphereRender_() {
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

    auto &addTile(glm::vec3 const &pos, int numericalValue) {
        Tile tile{};
        tile.numericalValue = numericalValue;
        tile.entityId = tfg::getEntityId();
        tile.colorPick = tfg::genPickColor(tile.entityId);

        // Setup cube
        tile.cube.scale = {1, 1, 0.375};
        tile.cube.color = {1, 0, 0};
        tile.updatePosition(pos);

        // Setup sphere
        for (auto &i: tile.sphere) {
            i.scale = glm::vec3{0.2f};
            i.color = {0, 1, 0};
        }

        tile.log();
        tileData_.emplace_back(tile);
        return tileData_[tileData_.size() - 1];
    }

    void drawBoard(bool isPicking = false) {

        planeRender_.draw(planeTransform, isPicking ? Injector::clearColor : planeTransform.color);

        for (auto &tile: tileData_) {
            glm::vec3 cubeColor;
            if (isPicking)
                cubeColor = tile.colorPick;
            else if (tile.isSelected)
                cubeColor = {0, 1, 1};
            else
                cubeColor = tile.cube.color;
            cubeRender_.draw(tile.cube, cubeColor);

            if (isPicking)
                continue;

            for (auto i: number2Braille_[tile.numericalValue]) {
                sphereRender_.draw(tile.sphere[i], tile.sphere[i].color);
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

    void moveTile(int entityiD, glm::vec3 pos) {
        auto a = getTile(entityiD);
        pos.z = a->cube.position.z;
        if (isInBoard(pos))
            a->updatePosition(pos);
    }


    [[nodiscard]] bool isInBoard(glm::vec3 const &position) const {
        if (!plane_.containsPoint(position)) return false;
//        if (position.x > planeTransform.scale.z) return false;
        return true;
    }


    void setupBoard() {
        // center the plane
        auto planeCenter = tileData_[(tileData_.size() - 1) / 2].cube.position;
        planeCenter.z -= tileData_[0].cube.scale.z;
        planeTransform.position = planeCenter;
        planeTransform.scale = {40, 40, 1};
        planeTransform.color = {0.6, 0.6, 0.6};

        // Calculate plane from tile positions
        auto p0 = tileData_[0].cube.position;
        auto p1 = tileData_[10].cube.position;
        auto p2 = tileData_[22].cube.position;
        plane_.fromPoints(p0, p1, p2);
        planeRender_.setPlane(plane_);

        // sanity check
        for (auto &t: tileData_) {
            assert(plane_.containsPoint(t.cube.position));
        }

        // setup drawer
        for (int i = 0; i < 10; ++i) {
            glm::vec3 pos{i * offset_, -9 * offset_, 0};
            addTile(pos, i).cube.color = {0, 0, 1};
        }

        LOG_INFO("{}", planeTransform.to_string().c_str());
        plane_.log();
    }

    Tile *nearestTile(glm::vec3 pos, int selectedEntity = -1) {
        Tile *nearest = nullptr;
        float best = INFINITY;
        for (auto &i: tileData_) {
//            LOG_DEBUG("Nearest {} {} {} {}", i.entityId,
//                      glm::to_string(pos).c_str(),
//                      glm::to_string(i.cube.position).c_str(),
//                      glm::distance(i.cube.position, pos));

            if (i.entityId == selectedEntity) continue;

            auto d = glm::distance(i.cube.position, pos);

            if (d < 2.0f && d < best) {
                best = d;
                nearest = &i;
            }
        }
        return nearest;
    }

    [[nodiscard]] const tfg::Plane &getPlane() const {
        return plane_;
    }

    const float offset_{4.0f};
private:
    std::vector<std::vector<size_t>> number2Braille_;
    std::vector<Tile> tileData_;
    tfg::Transform planeTransform{};
    tfg::Plane plane_;

    Cube cubeRender_;
    Plane planeRender_;
    Sphere sphereRender_;
};

#endif //TFG_BOARD_H
