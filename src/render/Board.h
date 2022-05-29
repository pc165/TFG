#ifndef TFG_BOARD_H
#define TFG_BOARD_H

#include "Sphere.h"
#include "Cube.h"
#include "Plane.h"
#include "Constants.h"

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

const std::vector<std::vector<int>> NUMBER_TO_BRAILLE = {
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

class Board {
public:
    using Tile = tfg::Tile;

    Board() : cubeRender_(), planeRender_(), sphereRender_() {

    }

    int addTile(glm::vec3 const &pos, int numericalValue) {
        Tile tile{};

        // setup basic data
        tile.numericalValue = numericalValue;
        tile.entityId = tfg::getEntityId();
        tile.colorPick = tfg::genPickColor(tile.entityId);
        tile.isDeck = false;

        // Setup cube
        tile.cube.scale = CUBE_SCALE;
        tile.cube.color = CUBE_COLOR_ASSIGNED;
        tile.updatePosition(pos);

        // Setup sphere
        for (auto &i: tile.sphere) {
            i.scale = SPHERE_CUBE;
            i.color = SPHERE_COLOR;
        }

        tileData_[tile.entityId] = tile;
        return tile.entityId;
    }

    void drawBoard(bool isPicking = false) {

        planeRender_.draw(planeTransform, isPicking ? Injector::clearColor : planeTransform.color, isPicking);

        for (auto &p: tileData_) {
            Tile &tile = p.second;
            glm::vec3 cubeColor;
            if (isPicking)
                cubeColor = tile.colorPick;
            else if (tile.isSelected)
                cubeColor = CUBE_COLOR_SELECTED;
            else
                cubeColor = tile.cube.color;
            cubeRender_.draw(tile.cube, cubeColor, isPicking);

            // skip rendering spheres
            if (isPicking)
                continue;

            auto const &spherePositions = NUMBER_TO_BRAILLE[tile.numericalValue];

            for (int i = 0; i < (int) spherePositions.size(); ++i) {
                if (i >= tile.hints && tile.hints != 0)
                    break;

                int sphereIdx = spherePositions[i];
                sphereRender_.draw(tile.sphere[sphereIdx], tile.sphere[sphereIdx].color, isPicking);
            }
        }
    }

    void setupDeck() {
        // setup plane
        auto planeCenter = tileData_[(tileData_.size() - 1) / 2].cube.position;
        planeCenter.z -= tileData_[0].cube.scale.z;
        planeTransform.position = planeCenter;
        planeTransform.scale = PLANE_SCALE;
        planeTransform.color = PLANE_COLOR;

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

            auto tile = getTile(addTile(pos, i));

            assert(tile != nullptr);

            tile->row = i;
            tile->col = -9;
            tile->cube.color = CUBE_COLOR_DECK;
            tile->isDeck = true;
        }

        LOG_INFO("{}", planeTransform.to_string().c_str());
        plane_.log();
    }

    Tile *nearestTile(glm::vec3 pos, std::function<bool(Tile const &)> const &predicate) {
        Tile *nearest = nullptr;
        float best = INFINITY;
        for (auto &i: tileData_) {
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


    [[nodiscard]] const tfg::Plane &getPlane() const {
        return plane_;
    }

    const float offset_{3.0f};
private:
    std::unordered_map<int, Tile> tileData_;
    tfg::Transform planeTransform{};
    tfg::Plane plane_;

    Cube cubeRender_;
    Plane planeRender_;
    Sphere sphereRender_;
};

#endif //TFG_BOARD_H
