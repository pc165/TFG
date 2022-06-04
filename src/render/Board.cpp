#include "Board.h"

int Board::addTile(const glm::vec3 &pos, int numericalValue) {
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

void Board::drawBoard(bool isPicking) {

    planeRender_.draw(planeTransform, isPicking ? GlobalOptions.clearColor : planeTransform.color, isPicking, GlobalOptions.drawPlaneNormals);
//        cubeRender_.draw(tfg::Transform(), tileData_[0].cube.color, isPicking, GlobalOptions.drawCubeNormals);
//        return;
    for (auto &p: tileData_) {
        Tile &tile = p.second;
        glm::vec3 cubeColor;
        if (isPicking)
            cubeColor = tile.colorPick;
        else if (tile.isSelected)
            cubeColor = CUBE_COLOR_SELECTED;
        else
            cubeColor = tile.cube.color;
        cubeRender_.draw(tile.cube, cubeColor, isPicking, GlobalOptions.drawCubeNormals);

        // skip rendering spheres
        if (isPicking)
            continue;

        auto const &spherePositions = NUMBER_TO_BRAILLE[tile.numericalValue];

        for (int i = 0; i < (int) spherePositions.size(); ++i) {
            if (i >= tile.hints && tile.hints != 0)
                break;

            int sphereIdx = spherePositions[i];
            sphereRender_.draw(tile.sphere[sphereIdx], tile.sphere[sphereIdx].color, isPicking, GlobalOptions.drawSphereNormals);
        }
    }
}

void Board::setupDeck() {
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

Board::Tile *Board::nearestTile(glm::vec3 pos, const std::function<bool(const Tile &)> &predicate) {
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

Board::Tile *Board::getTile(int entityId) {
    auto it = tileData_.find(entityId);
    if (it != tileData_.end())
        return &it->second;

    return nullptr;
}

bool Board::isInBoard(const glm::vec3 &position) const {
    if (!plane_.containsPoint(position)) return false;
    return true;
}

Board::Board() : cubeRender_(), planeRender_(), sphereRender_() {
    cubeRender_.setDiffuseMap(CUBE_COLOR_DEFAULT);
    planeRender_.setDiffuseMap(PLANE_COLOR);
    sphereRender_.setDiffuseMap(SPHERE_COLOR);
}
