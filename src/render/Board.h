#ifndef TFG_BOARD_H
#define TFG_BOARD_H

#include "Sphere.h"
#include "Cube.h"
#include "Plane.h"

struct Tile : tfg::Entity {
    int numericalValue{0};
    int row = -1, col = -1;
    bool isSelected{false};
    bool isDeck{false};
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

    void moveBack(float offset) {
        glm::vec3 pos{row * offset, col * offset, cube.position.z};
        updatePosition(pos);
    }


    void updatePosition(glm::vec3 pos) {
        pos.z = cube.position.z;
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
        tile.isDeck = false;
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
        tileData_[tile.entityId] = tile;
        return tileData_[tileData_.size() - 1];
    }

    void drawBoard(bool isPicking = false) {

        planeRender_.draw(planeTransform, isPicking ? Injector::clearColor : planeTransform.color);

        for (auto &p: tileData_) {
            Tile &tile = p.second;
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
        auto it = tileData_.find(entityId);
        if (it != tileData_.end())
            return &it->second;

        return nullptr;
    }

    [[nodiscard]] bool isInBoard(glm::vec3 const &position) const {
        if (!plane_.containsPoint(position)) return false;
//        if (position.x > planeTransform.scale.z) return false;
        return true;
    }


    void setupDeck() {
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
            assert(plane_.containsPoint(t.second.cube.position));
        }

        // setup deck
        for (int i = 0; i < 10; ++i) {
            glm::vec3 pos{i * offset_, -9 * offset_, 0};
            auto &tile = addTile(pos, i);
            tile.row = i;
            tile.col = -9;
            tile.cube.color = {0, 0, 1};
            tile.isDeck = true;
        }

        LOG_INFO("{}", planeTransform.to_string().c_str());
        plane_.log();
    }

    Tile *nearestTile(glm::vec3 pos, std::function<bool(Tile const &)> const &predicate) {
        Tile *nearest = nullptr;
        float best = INFINITY;
        for (auto &i: tileData_) {
//            LOG_DEBUG("Nearest {} {} {} {}", i.entityId,
//                      glm::to_string(pos).c_str(),
//                      glm::to_string(i.cube.position).c_str(),
//                      glm::distance(i.cube.position, pos));

            auto &tile = i.second;
            if (!predicate(tile)) continue;

            auto d = glm::distance(tile.cube.position, pos);

            if (d < 4.0f && d < best) {
                best = d;
                nearest = &tile;
            }
        }
        return nearest;
    }

    [[nodiscard]] const tfg::Plane &getPlane() const {
        return plane_;
    }

    const float offset_{3.0f};
private:
    std::vector<std::vector<size_t>> number2Braille_;
    std::unordered_map<int, Tile> tileData_;
    tfg::Transform planeTransform{};
    tfg::Plane plane_;

    Cube cubeRender_;
    Plane planeRender_;
    Sphere sphereRender_;
};

#endif //TFG_BOARD_H
