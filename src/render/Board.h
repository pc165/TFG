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

    Board();

    int addTile(glm::vec3 const &pos, int numericalValue);

    void drawBoard(bool isPicking = false);

    void setupDeck();

    Tile *nearestTile(glm::vec3 pos, std::function<bool(Tile const &)> const &predicate);

    Tile *getTile(int entityId);

    [[nodiscard]] bool isInBoard(glm::vec3 const &position) const;

    [[nodiscard]] const tfg::Plane &getPlane() const;

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
